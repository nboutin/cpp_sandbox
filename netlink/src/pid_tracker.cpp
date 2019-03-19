
#include "pid_tracker.h"

#include <errno.h>
#include <fstream>
#include <iostream>
#include <string.h>
#include <unistd.h>

#include <dirent.h>
#include <linux/cn_proc.h>
#include <linux/connector.h>
#include <linux/netlink.h>
#include <sys/socket.h>
#include <sys/types.h>

using namespace std;

constexpr size_t send_msg_len  = NLMSG_LENGTH(sizeof(cn_msg) + sizeof(proc_cn_mcast_op));
constexpr size_t rcv_msg_len   = NLMSG_LENGTH(sizeof(cn_msg) + sizeof(proc_event));
constexpr size_t send_msg_size = NLMSG_SPACE(send_msg_len);
constexpr size_t rcv_msg_size  = NLMSG_SPACE(rcv_msg_len);
const auto buffer_size         = max(max(send_msg_size, rcv_msg_size), (size_t) 1024);

PID_Tracker::PID_Tracker() : is_running(false)
{
    //    scan_proc();

    if(connect_to_netlink() == true)
    {
        thread = move(std::thread(&PID_Tracker::run, this));
        is_running.store(true);
    }
}

PID_Tracker::~PID_Tracker()
{
    is_running.store(false);
    thread.join();

    if(netlink_socket != -1)
    {
        if(shutdown(netlink_socket, SHUT_RDWR) < 0)
        {
            const auto err = errno;
            cerr << "Socket shutdown failed, errno: " << strerror(err) << endl;
        }

        if(close(netlink_socket) < 0)
        {
            const auto err = errno;
            cerr << "Socket close failed, errno: " << strerror(err) << endl;
        }
    }
}

//
// Private Definition
//

void PID_Tracker::run()
{
    while(is_running.load())
    {
        //
        // Receive
        //
        timeval timeout = {1, 0};

        fd_set read_fd_set;
        FD_ZERO(&read_fd_set);
        FD_SET(netlink_socket, &read_fd_set);

        if(select(netlink_socket + 1, &read_fd_set, nullptr, nullptr, &timeout) < 0)
        {
            const auto err = errno;
            cerr << "Socket select failed, errno: " << strerror(err) << endl;
            continue;
        }
        //        cout << "Socket select" << endl;

        if(FD_ISSET(netlink_socket, &read_fd_set))
        {
            char buffer[buffer_size];
            memset(buffer, 0x00, sizeof(char) * buffer_size);

            socklen_t socket_len;
            sockaddr_nl netlink_sockaddr = {AF_NETLINK, 0, 1, CN_IDX_PROC};

            auto read_size = recvfrom(netlink_socket,
                                      buffer,
                                      sizeof(char) * buffer_size,
                                      0,
                                      (sockaddr*) &netlink_sockaddr,
                                      &socket_len);
            if(read_size < 0)
            {
                const auto err = errno;
                cerr << "Socket recvfrom failed, errno: " << strerror(err) << endl;
                continue;
            }
            //            cout << "Socket recvfrom" << endl;

            nlmsghdr* netlink_msg = (nlmsghdr*) buffer;
            while(NLMSG_OK(netlink_msg, read_size))
            {
                if(netlink_msg->nlmsg_type == NLMSG_NOOP)
                {
                    continue;
                }

                if((netlink_msg->nlmsg_type == NLMSG_ERROR)
                   || (netlink_msg->nlmsg_type == NLMSG_OVERRUN))
                {
                    cerr << "NLMSG Error or Overrun" << endl;
                    break;
                }

                cn_msg* connector_msg = (cn_msg*) NLMSG_DATA(netlink_msg);
                proc_event* event     = (proc_event*) connector_msg->data;

                process_proc_event(event);

                if(netlink_msg->nlmsg_type == NLMSG_DONE)
                {
                    break;
                }
                netlink_msg = NLMSG_NEXT(netlink_msg, read_size);
            }
        }
    }
}

void PID_Tracker::process_proc_event(proc_event* event)
{
    cout << "cpu:" << event->cpu << ", time:" << event->timestamp_ns << " ";

    switch(event->what)
    {
    case proc_event::PROC_EVENT_EXEC:
        cout << "[Proc Event] EXEC, pid: " << event->event_data.exec.process_pid
             << ", tgid:" << event->event_data.exec.process_tgid << endl;
        break;

    case proc_event::PROC_EVENT_FORK:
        cout << "[Proc Event] FORK, pid: " << event->event_data.fork.child_pid
             << ", tgid:" << event->event_data.fork.child_tgid << endl;

        if(event->event_data.fork.child_pid == event->event_data.fork.child_tgid)
        {
            uint32_t pid = event->event_data.fork.child_pid;
            pids.insert(make_pair(pid, PID_t(pid)));
        }
        break;

    case proc_event::PROC_EVENT_EXIT:
        cout << "[Proc Event] EXIT, pid: " << event->event_data.exit.process_pid
             << ", tgid:" << event->event_data.exit.process_tgid << endl;

        if(event->event_data.exit.process_pid == event->event_data.exit.process_tgid)
        {
            auto it = pids.find(event->event_data.fork.child_pid);
            if(it != pids.end())
            {
                pids.erase(it);
            }
        }
        break;

    case proc_event::PROC_EVENT_UID: cout << "[Proc Event] UID" << endl; break;
    case proc_event::PROC_EVENT_GID:
        cout << "[Proc Event] GID, pid:" << event->event_data.id.process_pid
             << ", tgid:" << event->event_data.id.process_tgid
             << ", rgid:" << event->event_data.id.r.rgid << ", egid:" << event->event_data.id.e.egid
             << endl;
        break;
    case proc_event::PROC_EVENT_NONE: cout << "[Proc Event] NONE" << endl; break;
    case proc_event::PROC_EVENT_COMM:
        cout << "[Proc Event] COMM, pid:" << event->event_data.comm.process_pid
             << ", tgid:" << event->event_data.comm.process_tgid
             << ", comm:" << event->event_data.comm.comm << endl;
        break;

    default: cout << "[Proc Event] Default " << std::hex << event->what << std::dec << endl; break;
    }
}

bool PID_Tracker::connect_to_netlink()
{
    //
    // Create socket
    //
    netlink_socket = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_CONNECTOR);
    if(netlink_socket < 0)
    {
        auto err = errno;
        cerr << "Socket (NETLINK_CONNECTOR) creation failed, errno: " << strerror(err) << endl;
        return false;
    }
    cout << "Socket (NETLINK_CONNECTOR) creation" << endl;

    //
    // Bind socket
    //
    sockaddr_nl connector_sockaddr;
    connector_sockaddr.nl_family = AF_NETLINK;
    connector_sockaddr.nl_groups = CN_IDX_PROC;
    connector_sockaddr.nl_pid    = getpid();
    if(bind(netlink_socket,
            reinterpret_cast<sockaddr*>(&connector_sockaddr),
            sizeof(connector_sockaddr))
       < 0)
    {
        const auto err = errno;
        cerr << "Socket bind failed, errno: " << strerror(err) << endl;
        return false;
    }
    cout << "Socket bind" << endl;

    //
    // Send to socket
    //

    // Netlink Header
    char buffer[buffer_size];
    memset(buffer, 0x00, sizeof(char) * buffer_size);
    nlmsghdr* netlink_header   = reinterpret_cast<nlmsghdr*>(buffer);
    netlink_header->nlmsg_len  = send_msg_len;
    netlink_header->nlmsg_type = NLMSG_DONE;
    netlink_header->nlmsg_pid  = getpid();

    // Connector message
    cn_msg* connector_msg  = reinterpret_cast<cn_msg*>(NLMSG_DATA(netlink_header));
    connector_msg->data[0] = PROC_CN_MCAST_LISTEN;
    connector_msg->id.idx  = CN_IDX_PROC;
    connector_msg->id.val  = CN_VAL_PROC;
    connector_msg->len     = sizeof(proc_cn_mcast_op);

    if(send(netlink_socket, netlink_header, netlink_header->nlmsg_len, 0)
       != static_cast<int>(netlink_header->nlmsg_len))
    {
        const auto err = errno;
        cerr << "Socket send failed, errno: " << strerror(err) << endl;
        return false;
    }
    cout << "Socket send" << endl;
    return true;
}

void PID_Tracker::scan_proc()
{
    // TODO Test with system command
    // top -b -n1
    // ps aux
    //	string result = exec("ps -A -o pid");

    DIR* dir = opendir("/proc/");

    if(nullptr == dir)
    {
        auto err = errno;
        cerr << __PRETTY_FUNCTION__ << " cannot open directory '/proc' " << strerror(err) << endl;
        return;
    }

    dirent* dirp = nullptr;
    while((dirp = readdir(dir)) != nullptr)
    {
        if(is_valid_process(dirp->d_name))
        {
            const uint32_t pid = stoi(dirp->d_name);
            pids.insert(make_pair(pid, PID_t(pid)));
        }
        else
        {
            cerr << dirp->d_name << " is not a process" << endl;
        }
    }

    if(0 != closedir(dir))
    {
        cerr << __PRETTY_FUNCTION__ << " cannot close directory '/proc'" << endl;
    }

    for(auto p : pids)
        cout << p.first << " ";
    cout << endl;
}

bool PID_Tracker::is_valid_process(const string& pid)
{
    //	/proc/[pid]/cmdline
    //	              This read-only file holds the complete command line for the
    //	              process, unless the process is a zombie.  In the latter case,
    //	              there is nothing in this file

    const string cmdline_path = "/proc/" + pid + "/cmdline";
    fstream f_cmdline(cmdline_path, f_cmdline.in | f_cmdline.binary);
    if(!f_cmdline.is_open())
        return false;

    // Does not contains "self"
    // Avoid self and thread-self
    if(pid.find("self") != string::npos)
        return false;

    const string status_path = "/proc/" + pid + "/status";
    fstream f_status(status_path, f_status.in | f_status.binary);
    if(!f_status.is_open())
        return false;

    return true;
}
