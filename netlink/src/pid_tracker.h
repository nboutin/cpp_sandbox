
#pragma once

#include <atomic>
#include <map>
#include <string>
#include <thread>

#include <linux/cn_proc.h>

struct PID_t
{
    explicit PID_t(uint32_t pid) : pid(pid) {}
    uint32_t pid = 0;
};

class PID_Tracker
{
public:
    PID_Tracker();
    ~PID_Tracker();

private:
    void scan_proc();
    bool is_valid_process(const std::string& pid);
    bool connect_to_netlink();

    void run();
    void process_proc_event(proc_event* event);

    std::map<uint32_t, PID_t> pids;
    int netlink_socket = -1;

    std::thread thread;
    std::atomic_bool is_running;
};
