#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
int main() {
    std::cout << "客户端启动" << std::endl;
    int cli_fd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in ser_addr;
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_addr.s_addr = inet_addr("192.168.184.134");
    ser_addr.sin_port = htons(6969);

    while (1)
    {
        std::string buf;
        std::cout << "小客:";
        std::cin >> buf;
        sendto(cli_fd, buf.c_str(), sizeof(buf), 0, (struct sockaddr*)&ser_addr, sizeof(ser_addr));

        char recvBuf[1024]{};
        socklen_t ser_len = sizeof(ser_addr);
        recvfrom(cli_fd, recvBuf, sizeof(recvBuf), 0, (struct sockaddr*)&ser_addr, &ser_len);

        std::cout << "小服:" << recvBuf << std::endl;
    }
    close(cli_fd);
    return 0;
}