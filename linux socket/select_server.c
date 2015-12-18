/*通过select来实现网络I/O的复用*/

#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#define MAX_LISTEN 10
#define PORT 8888
#define BUFF_SIZE 1024

int fd_SET[MAX_LISTEN];
int conn_amount;

int main()
{
	int server_fd, new_fd;
	int err, ret, maxsock, i;
	int reuse = 1;
	fd_set fdsr;
	char buf[BUFF_SIZE];
	socklen_t clientaddr_len = sizeof(struct sockaddr_in);
	struct sockaddr_in serveraddr;
	struct sockaddr_in clientaddr;
	struct timeval timev;

	//create socket
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(server_fd == -1)
	{
		perror("socket failed");
		exit(errno);
	}

	//set server IP
	memset(&serveraddr, 0, sizeof(serveraddr));
	memset(&clientaddr, 0, sizeof(clientaddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(PORT);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	memset(serveraddr.sin_zero, 0, 8);

	//bind IP and PORT
	err = bind(server_fd, (struct sockaddr*)&serveraddr, sizeof(struct sockaddr));
	if(err == -1)
	{
		perror("bind error");
		exit(errno);
	}

	//listen
	err = listen(server_fd, MAX_LISTEN);
	if(err == -1)
	{
		perror("listen error");
		exit(errno);
	}

	maxsock = server_fd;
	conn_amount = 0;

	while(1)
	{
		//initialize file descriptor set
		FD_ZERO(&fdsr);
		FD_SET(server_fd, &fdsr);

		//timeout setting
		timev.tv_sec = 30;
		timev.tv_usec = 0;

		//add active connection to fd set
		for(i = 0; i < MAX_LISTEN; i++)
		{
			if(fd_SET[i] != 0)
			{
				FD_SET(fd_SET[i], &fdsr);
			}
		}

		//select
		ret = select(maxsock+1, &fdsr, NULL, NULL, &timev);
		if(ret < 0)
		{
			perror("select error");
			exit(1);
		}
		else if(ret == 0)
		{
			printf("timeout\n");
			continue;
		}

		//check every fd in the set
		for(i = 0; i < conn_amount; i++)
		{
			if(FD_ISSET(fd_SET[i], &fdsr))
			{
				ret = recv(fd_SET[i], buf, sizeof(buf), 0);
				if(ret <= 0)
				{
					perror("receive data error");
					printf("close client[%d]\n", i);
					close(fd_SET[i]);
					FD_CLR(fd_SET[i], &fdsr);
					fd_SET[i] = 0;
				}
				else 
				{
					if(ret < BUFF_SIZE)
					{
					    buf[ret] = '\0';
						send(fd_SET[i], buf, strlen(buf), 0);
					}
					printf("receive from client[%d] is %s\n", i, buf);
				}
			}
		}

		//check whether a new connection comes
		if(FD_ISSET(server_fd, &fdsr))
		{
			new_fd = accept(server_fd, (struct sockaddr*)&clientaddr, &clientaddr_len);
			if(new_fd < 0)
			{
				perror("accept error");
				continue;
			}

			//add the new fd to fd set
			if(conn_amount < MAX_LISTEN)
			{	
				fd_SET[conn_amount++] = new_fd;
				printf("new connection client[%d]:IP:%s PORT:%d\n", conn_amount, inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
				if(new_fd > maxsock)
				{
					maxsock = new_fd;
				}
			}
			else
			{
				printf("the connections have reached the upper limit,exit\n");
				send(new_fd, "bye", 4, 0);
				close(new_fd);
				break;
			}
		}
	}

	//close all fd
	for(i = 0; i < MAX_LISTEN; i++)
	{
		if(fd_SET[i] != 0)
		close(fd_SET[i]);
	}

	return 0;
}
