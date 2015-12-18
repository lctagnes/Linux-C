/*简单的UDP服务端程序*/

#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<netinet/in.h>
#define BUFFER_SIZE 1024
int main()
{
	int server_fd;
	int err, clientaddr_len;
	struct sockaddr_in myaddr;
	struct sockaddr_in clientaddr;
	char buf[BUFFER_SIZE];

	server_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(server_fd == -1)
	{
		perror("socket failed");
		exit(1);
	}

	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(8000);
	myaddr.sin_addr.s_addr = inet_addr("192.168.2.110");
	memset(myaddr.sin_zero, 0, 8);

	err = bind(server_fd, (struct sockaddr*)&myaddr, sizeof(struct sockaddr));
	if(err == -1)
	{
		perror("bind error");
		exit(1);
	}

	while(1)
	{
	clientaddr_len = sizeof(struct sockaddr_in);
	err = recvfrom(server_fd, buf, BUFFER_SIZE, 0, (struct sockaddr*)&clientaddr, &clientaddr_len);
	if(err == -1)
	{
		perror("receive data failed");
		exit(1);
	}

	printf("receive from %s,port %d, string is %s\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), buf);
	sendto(server_fd, buf, sizeof(buf), 0, (struct sockaddr*)&clientaddr, clientaddr_len);
	}

	close(server_fd);

	return 0;

}
