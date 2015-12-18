/*简单的UDP客户端程序*/
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
	int client_fd;
	int err, clientaddr_len;
	struct sockaddr_in server_addr;
	char buf[BUFFER_SIZE];

	client_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(client_fd == -1)
	{
		perror("socket failed");
		exit(1);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(8000);
	server_addr.sin_addr.s_addr = inet_addr("192.168.2.110");
	memset(server_addr.sin_zero, 0, 8);

	while(1)
	{
	printf("send something:\n");
	scanf("%s",&buf);
	sendto(client_fd, buf, sizeof(buf), 0, (struct sockaddr*)&server_addr, sizeof(struct sockaddr));
	memset(buf, 0, sizeof(buf));
	recvfrom(client_fd, buf, BUFFER_SIZE, 0, NULL, NULL);
	printf("receive from server:%s\n", buf);
	}
	close(client_fd);

	return 0;

}
