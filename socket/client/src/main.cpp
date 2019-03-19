#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <unistd.h>

using namespace std;

int main(int argc, char* argv[])
{
    if(argc < 3)
    {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(0);
    }

    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_fd < 0)
    {
        perror("ERROR socket");
        return EXIT_FAILURE;
    }

    struct hostent* server = gethostbyname(argv[1]);
    if(server == nullptr)
    {
        cerr << "ERROR no such host" << endl;
        return EXIT_FAILURE;
    }

    struct sockaddr_in serv_addr;
    bzero((char*) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char*) server->h_addr, (char*) &serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(std::stoi(argv[2]));

    if(connect(socket_fd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("ERROR connect");
        return EXIT_FAILURE;
    }

    while(true)
    //    for(int i = 0; i < 10; ++i)
    {
        //        printf("Please enter the message: ");
        char buffer[256];
        //        bzero(buffer, 256);
        //        fgets(buffer, 255, stdin);
        //
        //        int n = write(socket_fd, buffer, strlen(buffer));
        //        if(n < 0)
        //            error("ERROR writing to socket");

        bzero(buffer, 256);
        int n = read(socket_fd, buffer, 255);

        if(n < 0)
        {
            perror("ERROR read");
            continue;
        }

        //        printf("%s\n", buffer);
        cout << "read:" << buffer << endl;
    }

    shutdown(socket_fd, SHUT_RDWR);
    close(socket_fd);
    return 0;
}
