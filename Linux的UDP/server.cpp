#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
int main() {
    int ser_fd;
    struct sockaddr_in ser_addr, cli_addr;
    ser_fd = socket(AF_INET, SOCK_DGRAM, 0);
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_addr.s_addr = INADDR_ANY;
    ser_addr.sin_port = htons(6969);

    bind(ser_fd, (struct sockaddr*)&ser_addr, sizeof(ser_addr));

    std::cout << "服务器启动" << std::endl;

    while (1) {
        char recvBuf[1024]{};
        socklen_t len = sizeof(cli_addr);
        recvfrom(ser_fd, recvBuf, sizeof(recvBuf), 0, (struct sockaddr*)&cli_addr, &len);
        std::cout << "小客:" << recvBuf << std::endl;

        std::string message;
        std::cout << "我:";
        std::cin >> message;
        sendto(ser_fd, message.c_str(), sizeof(message), 0, (struct sockaddr*)&cli_addr, len);
    }
    close(ser_fd);
    return 0;
}