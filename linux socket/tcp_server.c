/*简单的TCP服务端程序*/
#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<netinet/in.h>

#define MAX_LISTEN 10

int main()
{
	int server_fd, client_fd;
	int err, clientaddr_len;
	struct sockaddr_in myaddr;
	char buf[100] = "welcome to server!\n";
	struct sockaddr_in clientaddr;

	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(server_fd == -1)
	{
		perror("socket failed");
		exit(1);
	}

	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(8080);
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

	clientaddr_len = sizeof(struct sockaddr_in);
	client_fd = accept(server_fd, (struct sockaddr*)&clientaddr, &clientaddr_len);
	if(client_fd == -1)
	{
		perror("accept error");
		exit(1);
	}

	write(client_fd, buf, 100);
	close(server_fd);
	close(client_fd);

	return 0;

}
