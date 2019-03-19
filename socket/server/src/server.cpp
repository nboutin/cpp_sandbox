
#include "server.h"

#include <cstring>
#include <iostream>
#include <string>
#include <thread>

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

constexpr int PENDING_CONNECTION_MAX = 5;

Server::Server(int port)
{
    if((socket_fd = ::socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("ERROR socket");
        return;
    }

    struct sockaddr_in serv_addr;
    bzero((char*) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family      = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port        = htons(port);

    if(bind(socket_fd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("ERROR bind");
        return;
    }

    if(listen(socket_fd, PENDING_CONNECTION_MAX) < 0)
    {
        perror("ERROR listen");
        return;
    }

    auto t = thread(&Server::run, this);
    t.detach();
}

void Server::run()
{
    while(true)
    {
        struct sockaddr_in cli_addr;
        socklen_t clilen  = sizeof(cli_addr);
        int client_socket = 0;

        if((client_socket = accept(socket_fd, (struct sockaddr*) &cli_addr, &clilen)) < 0)
        {
            perror("ERROR accept");
            continue;
        }

        clients.push_back(client_socket);
        auto t = std::thread(&Server::client_thread, this, client_socket);
        t.detach();
    }
}

Server::~Server() { close(socket_fd); }

void Server::write(const std::string& s)
{
    for(auto c : clients)
    {
        int n = ::write(c, s.c_str(), s.size());

        if(n < 0)
        {
            perror("ERROR write");
            continue;
        }
    }
}

void Server::client_thread(int client_socket)
{
    if(client_socket < 0)
        cerr << "ERROR client_socket" << endl;

    char buffer[64];
    while(true)
    {
        bzero(buffer, 64);
        int n = read(client_socket, buffer, sizeof(buffer));
        if(n < 0)
        {
            perror("ERROR read");
            continue;
        }
        else if(n == 0)
        {
            cout << "Disconnect:" << client_socket << endl;
            clients.remove(client_socket);
            break;
        }
        cout << "Read(" << n << "):[" << buffer << "]" << endl;
    }
    close(client_socket);
}
