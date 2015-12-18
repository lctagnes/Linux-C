/*服务器端创建子进程为客户端服务*/
#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<netinet/in.h>

#define PORT 8080
#define MAX_LISTEN 10
#define BUFF_SIZE 1024

int main()
{
	int server_fd, client_fd;
	int err, clientaddr_len;
	pid_t pid;
	char buf[BUFF_SIZE];
	int bytes;
	struct sockaddr_in myaddr;
	struct sockaddr_in clientaddr;

	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(server_fd == -1)
	{
		perror("socket failed");
		exit(1);
	}

	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(PORT);
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	memset(myaddr.sin_zero, 0, 8);

	err = bind(server_fd, (struct sockaddr*)&myaddr, sizeof(struct sockaddr));
	if(err == -1)
	{
		perror("bind error");
		exit(1);
	}

	err = listen(server_fd, MAX_LISTEN);
	if(err == -1)
	{
		perror("listen error");
		exit(1);
	}

	while(1)
	{
		clientaddr_len = sizeof(struct sockaddr_in);
		client_fd = accept(server_fd, (struct sockaddr*)&clientaddr, &clientaddr_len);
		if(client_fd == -1)
		{
			perror("accept error");
			exit(1);		
		}

		pid = fork();
		if(pid == 0)
		{
			close(server_fd);
			while(bytes = read(client_fd, buf, BUFF_SIZE))
			{
				write(client_fd, buf, bytes);
				printf("receive from client:%s\n", buf);
			}
			if(bytes <= 0)
			{
				perror("read error");
				continue;
			}
		}
		close(client_fd);
	}

	return 0;
}
