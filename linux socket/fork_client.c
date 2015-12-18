/*服务器端创建子进程为客户端服务*/
#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<netinet/in.h>
#define BUFF_SIZE 1024

int main()
{
	int client_fd;
	int err, bytes;
	struct sockaddr_in clientaddr;
	char readbuf[BUFF_SIZE];
	char writebuf[BUFF_SIZE];

	client_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(client_fd == -1)
	{
		perror("socket failed");
		exit(1);
	}

	clientaddr.sin_family = AF_INET;
	clientaddr.sin_port = htons(8080);
	clientaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	memset(clientaddr.sin_zero, 0, 8);

	err = connect(client_fd, (struct sockaddr*)&clientaddr, sizeof(struct sockaddr));
	if(err == -1)
	{
		perror("connect error");
		exit(1);
	}

	while(fgets(writebuf, BUFF_SIZE, stdin))
	{
		write(client_fd, writebuf, strlen(writebuf));
		bytes = read(client_fd, readbuf, BUFF_SIZE);
		if(bytes <= 0)
		{
			perror("read error");
			continue;
		}
		readbuf[bytes] = '\0';
		printf("receive from server:%s\n", readbuf);
	}
	close(client_fd);
	return 0;
}
