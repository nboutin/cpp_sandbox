
#pragma once

#include <string>
#include <list>

class Server
{
public:
    Server(int port);
    ~Server();

    void write(const std::string& s);

private:
    void run();
    void client_thread(int socket);

    int socket_fd = 0;
    std::list<int> clients;
};
