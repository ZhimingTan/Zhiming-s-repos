#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
int main() {
    int cli_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in ser_addr;

    ser_addr.sin_addr.s_addr = inet_addr("192.168.184.134");
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_port = htons(6969);

    int ret = connect(cli_socket, (struct sockaddr*)&ser_addr, sizeof(ser_addr));
    if (ret == -1) {
        std::cout << "链接失败" << std::endl;
    }
    else {
        std::cout << "连接成功" << std::endl;
    }

    char buf[1024]{};
    while (1) {
        std::cout << "client:";
        memset(buf, 0, sizeof(buf));
        std::cin >> buf;
        send(cli_socket, buf, sizeof(buf), 0);

        recv(cli_socket, buf, sizeof(buf), 0);
        std::cout << "server:" << buf << std::endl;
    }
    close(cli_socket);
    return 0;
}