#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
int main() {
    int ser_socket, cli_socket;
    struct sockaddr_in ser_addr, cli_addr;
    ser_socket = socket(AF_INET, SOCK_STREAM, 0);
    ser_addr.sin_addr.s_addr = INADDR_ANY;
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_port = htons(6969);
    char buf[1024]{};

    bind(ser_socket, (struct sockaddr*)&ser_addr, sizeof(ser_addr));

    listen(ser_socket, 5);

    std::cout << "server is listening" << std::endl;

    socklen_t len = sizeof(cli_addr);
    cli_socket = accept(ser_socket, (struct sockaddr*)&cli_addr, &len);

    while (1) {
        memset(buf, 0, sizeof(buf));
        recv(cli_socket, buf, sizeof(buf), 0);
        std::cout << "client:" << buf << std::endl;

        std::cout << "server:";
        std::cin >> buf;
        send(cli_socket, buf, sizeof(buf), 0);

    }
    close(ser_socket);
    close(cli_socket);
    return 0;
}