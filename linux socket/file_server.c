/*************************************************************************
	> File Name: file_server.c
	> Author: CuitingLin
	> Mail: agnes2729@sina.com 
	> Created Time: Wed 01 Jul 2015 02:24:42 PM CST
 ************************************************************************/
  /*		编写tcp文件服务器和客户端。客户端可以上传和下载文件
	客户端功能如下：
		1. 支持以下命令
			help : 显示客户端所有命令和说明
			list : 显示服务器端可下载文件列表
			get <file> : 下载文件
			put <file> : 上传文件
			quit : 退出客户端

	服务器端功能(单进程)：
		解析客户端命令并提供相应服务*/
#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<errno.h>
#include<unistd.h>
#include<fcntl.h>

#define N 256

void do_list(int connfd)
{
	DIR *mydir = NULL;
	struct dirent *dp;

	if((mydir = opendir(".")) == NULL)
	{
		perror("opendir failed");
		exit(1);
	}
	while(dp = readdir(mydir))
	{
		if(dp->d_name[0] != '.')
		{
			write(connfd, dp->d_name, N);
		}
	}

	closedir(mydir);
}

void do_get(int connfd, char fname[])
{
	int fd, nbytes;
	char buf[N];

	fd = open(fname, O_RDONLY);
	if(fd < 0)
	{
		perror("open file failed");
		write(connfd, "N", 1);
		return;
	}
	else
	{
		printf("open file sucess! send 'Y' to client\n");
		write(connfd, "Y", 1);
	}

	while(nbytes = read(fd, buf, N))
	{
		write(connfd, buf, nbytes);
	}
	printf("get file %s sucess\n", fname);

	close(fd);
}

void do_put(int connfd, char fname[])
{
	int fd, nbytes;
	char buf[N];

	fd = open(fname, O_WRONLY|O_CREAT|O_EXCL, 0666);
	if(fd < 0)
	{
		perror("open file failed");
		write(connfd,  "N", 1);
		return;
	}
	else
	{
		printf("open file sucess! send 'Y' to client\n");
		write(connfd, "Y", 1);
	}

	while(nbytes = read(connfd, buf, N))
	{
		write(fd, buf, nbytes);
	}

	printf("put file sucess\n");

	close(fd);
}

int main(int argc, char *argv[])
{
	int serverfd, connfd;
	struct sockaddr_in serveraddr;
	struct sockaddr_in clientaddr;
	int err, clientaddr_len;
	char command[N];

	if(argc < 3)
	{
		printf("usage:%s ip port\n", argv[0]);
		return -1;
	}

	serverfd = socket(AF_INET, SOCK_STREAM, 0);
	if(serverfd == -1)
	{
		perror("socket error");
		exit(1);
	}

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(atoi(argv[2]));
	serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
	memset(serveraddr.sin_zero, 0, 8);

	err = bind(serverfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if(err == -1)
	{
		perror("bind error");
		exit(1);
	}

	err = listen(serverfd, 10);
	if(err == -1)
	{
		perror("listen error");
		exit(1);
	}

	while(1)
	{
	clientaddr_len = sizeof(struct sockaddr_in);
	connfd = accept(serverfd, (struct sockaddr*)&clientaddr, &clientaddr_len);
	if(connfd == -1)
	{
		perror("accept error");
		exit(1);
	}
	read(connfd, command, N);
	switch(command[0])
	{
		case 'l':
			do_list(connfd);
			break;
		case 'g':
			do_get(connfd,command+1);
			break;
		case 'p':
			do_put(connfd,command+1);
			break;
	}
	close(connfd);
	}

	close(serverfd);

	return 0;
}
