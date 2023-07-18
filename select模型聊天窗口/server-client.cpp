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
	/*����һ��socket����tcpͨ��*/
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		exit(errno);
	}

	/*��ʼ����������(�Է�)�ĵ�ַ�Ͷ˿���Ϣ*/
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
		/*�Ѽ������*/
		FD_ZERO(&rfds);
		/*�ѱ�׼������0���뵽������*/
		FD_SET(0, &rfds);
		maxfd = 0;
		/*�ѵ�ǰ���Ӿ��socket ���뵽������*/
		FD_SET(sockfd, &rfds);
		if (sockfd > maxfd)
			maxfd = sockfd;
		/*�������ȴ�ʱ��*/
		tv.tv_sec = 1;
		tv.tv_usec = 0;
		/*��ʼ�ȴ�*/
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
				/*���ӵ�socket ������Ϣ����ղ���ʾ*/
				bzero(buffer, MAXBUF + 1);
				/*���նԷ�����������Ϣ�����MAXBUF�ֽ�*/
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
			}// FD_ISSET = sockfd���
			if (FD_ISSET(0, &rfds)) {
				/*�û��������ˣ����ȡ�����ݲ�����*/
				bzero(buffer, MAXBUF + 1);
				fgets(buffer, MAXBUF, stdin);
				if (!strncasecmp(buffer, "quit", 4)) {
					printf("self request to quit the chating\n");
					break;
				}
				/*����Ϣ��������*/
				len = send(sockfd, buffer, strlen(buffer) - 1, 0);
				if (len < 0) {
					printf("mgs:%s send fail!errno is:%d,error info is:%s\n", buffer, errno, strerror(errno));
					break;
				}
				else {
					printf("msg: %s\t send success, send %d bytes!\n", buffer, len);
				}
			}//FD_ISSET = 0

		}//select �������

	}//���������while ѭ��
	/*�ر�����*/
	close(sockfd);
	return 0;
}