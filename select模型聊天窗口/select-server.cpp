#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <pthread.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAXBUF 1024

int main(int argc, char** argv) {
	int sockfd, new_fd;
	socklen_t len;
	struct sockaddr_in my_addr, their_addr;
	unsigned int myport, lisnum;
	char buf[MAXBUF + 1];
	fd_set rfds;
	struct timeval tv;
	int retval, maxfd = -1;

	if (argv[1]) {
		myport = atoi(argv[1]);
	}
	else
		myport = 7838;

	if (argv[2]) {
		lisnum = atoi(argv[2]);
	}
	else
		lisnum = 2;
	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	bzero(&my_addr, sizeof(my_addr));
	my_addr.sin_family = PF_INET;
	my_addr.sin_port = htons(myport);
	if (argv[3])
		my_addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(sockfd, (struct sockaddr*)&my_addr, sizeof(struct sockaddr)) == -1) {
		perror("bind");
		exit(1);
	}

	if (listen(sockfd, lisnum) == -1) {
		perror("listen");
		exit(1);
	}

	while (1) {
		printf("\n----waiting for new connecting to start new char----\n");
		len = sizeof(struct sockaddr);
		if ((new_fd = accept(sockfd, (struct sockaddr*)&their_addr, &len)) == -1) {
			perror("accept");
			exit(errno);
		}
		else
			printf("server:got connection from %s,port %d,socked %d\n", \
				inet_ntoa(their_addr.sin_addr), \
				ntohs(their_addr.sin_port), new_fd);

		/*开始处理每个新连接上的数据收发*/
		//printf("\n---ready to go.now you can chatting...input enter,then you can chat---\n");
		while (1) {
			/*把集合清空*/
			FD_ZERO(&rfds);
			/*把标准输入句柄0加入到集合中*/
			FD_SET(0, &rfds);
			maxfd = 0;
			/*把当前连接句柄new_fd加入到集合中*/
			FD_SET(new_fd, &rfds);
			if (new_fd > maxfd)
				maxfd = new_fd;
			/*设置最大等待时间*/
			tv.tv_sec = 1;
			tv.tv_usec = 0;

			retval = select(maxfd + 1, &rfds, NULL, NULL, &tv);
			if (retval == -1) {
				printf("select error!%s\n", strerror(errno));
				break;
			}
			else if (retval == 0) {
				//printf("no message come,user no press the button,please wait...\n");
				continue;
			}
			else {

				if (FD_ISSET(new_fd, &rfds)) {
					/*连接的socket 上有消息则接收并显示*/
					bzero(buf, MAXBUF + 1);
					/*接收对方发过来的消息，最多MAXBUF字节*/
					len = recv(new_fd, buf, MAXBUF, 0);
					if (len > 0)
						printf("recv msg success:%s! %d bytes rcv.\n ", buf, len);
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
					bzero(buf, MAXBUF + 1);
					fgets(buf, MAXBUF, stdin);
					if (!strncasecmp(buf, "quit", 4)) {
						printf("self request to quit the chating\n");
						break;
					}
					/*发消息给服务器*/
					len = send(new_fd, buf, strlen(buf) - 1, 0);
					if (len < 0) {
						printf("mgs:%s send fail!errno is:%d,error info is:%s\n", buf, errno, strerror(errno));
						break;
					}
					else {
						printf("msg: %s\t send success, send %d bytes!\n", buf, len);
					}
				}//FD_ISSET = 0

			}//select 处理结束

		}/*内while*/
		close(new_fd);
		/*处理每个新连接上的数据收发结束*/
		printf("would you want to chatting with another one?(no->quit)");
		fflush(stdout);
		bzero(buf, MAXBUF + 1);
		fgets(buf, MAXBUF, stdin);
		if (!strncasecmp(buf, "no", 2)) {
			printf("quit the chatting!\n");
			break;
		}

	}/*外while*/

	close(sockfd);
	return 0;
}