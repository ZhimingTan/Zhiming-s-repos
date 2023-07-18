#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <pthread.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <resolv.h>

#define MAXBUF 1024
int main(int argc, char** argv) {
	int sockfd, len;
	struct sockaddr_in dest;
	char buffer[MAXBUF + 1];
	fd_set rfds;
	struct timeval tv;
	int retval, maxfd = -1;

	if (argc != 3) {
		printf("the param style wrong!\n");
		exit(0);
	}
	/*创建一个socket用于tcp通信*/
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		exit(errno);
	}

	/*初始化服务器端(对方)的地址和端口信息*/
	bzero(&dest, sizeof(dest));
	dest.sin_family = AF_INET;
	dest.sin_port = htons(atoi(argv[2]));
	if (inet_aton(argv[1], (struct in_addr*)&dest.sin_addr.s_addr) == 0) {
		perror(argv[1]);
		exit(errno);
	}

	/*conect to the server*/
	if (connect(sockfd, (struct sockaddr*)&dest, sizeof(dest)) != 0) {
		perror("connect");
		exit(errno);
	}

	printf("\n---ready to chatting...---\n");
	while (1) {
		/*把集合清空*/
		FD_ZERO(&rfds);
		/*把标准输入句柄0加入到集合中*/
		FD_SET(0, &rfds);
		maxfd = 0;
		/*把当前连接句柄socket 加入到集合中*/
		FD_SET(sockfd, &rfds);
		if (sockfd > maxfd)
			maxfd = sockfd;
		/*设置最大等待时间*/
		tv.tv_sec = 1;
		tv.tv_usec = 0;
		/*开始等待*/
		retval = select(maxfd + 1, &rfds, NULL, NULL, &tv);
		if (retval == -1) {
			printf("select error,quit!\n");
			break;
		}
		else if (retval == 0) {
			continue;
		}
		else {
			if (FD_ISSET(sockfd, &rfds)) {
				/*连接的socket 上有消息则接收并显示*/
				bzero(buffer, MAXBUF + 1);
				/*接收对方发过来的消息，最多MAXBUF字节*/
				len = recv(sockfd, buffer, MAXBUF, 0);
				if (len > 0)
					printf("recv msg success:%s! %d bytes rcv.\n ", buffer, len);
				else {
					if (len < 0) {
						printf("recv msg fail.the errno is:%d,error info is %s.\n", errno, strerror(errno));
					}
					else
						printf("quit.\n");
					break;
				}
			}// FD_ISSET = sockfd情况
			if (FD_ISSET(0, &rfds)) {
				/*用户有输入了，则读取其内容并发送*/
				bzero(buffer, MAXBUF + 1);
				fgets(buffer, MAXBUF, stdin);
				if (!strncasecmp(buffer, "quit", 4)) {
					printf("self request to quit the chating\n");
					break;
				}
				/*发消息给服务器*/
				len = send(sockfd, buffer, strlen(buffer) - 1, 0);
				if (len < 0) {
					printf("mgs:%s send fail!errno is:%d,error info is:%s\n", buffer, errno, strerror(errno));
					break;
				}
				else {
					printf("msg: %s\t send success, send %d bytes!\n", buffer, len);
				}
			}//FD_ISSET = 0

		}//select 处理结束

	}//处理聊天的while 循环
	/*关闭连接*/
	close(sockfd);
	return 0;
}