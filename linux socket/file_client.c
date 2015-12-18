/*************************************************************************
	> File Name: file_server.c
	> Author: CuitingLin
	> Mail: agnes2729@sina.com 
	> Created Time: Wed 01 Jul 2015 02:24:42 PM CST
 ************************************************************************/
 /*		��дtcp�ļ��������Ϳͻ��ˡ��ͻ��˿����ϴ��������ļ�
	�ͻ��˹������£�
		1. ֧����������
			help : ��ʾ�ͻ������������˵��
			list : ��ʾ�������˿������ļ��б�
			get <file> : �����ļ�
			put <file> : �ϴ��ļ�
			quit : �˳��ͻ���

	�������˹���(������)��
		�����ͻ�������ṩ��Ӧ����*/
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


void do_help()
{
	printf("	  help : display help info\n");
	printf("	  list : display file list from server\n");
	printf("get <file> : download file from server\n ");
	printf("put <file> : upload file to server\n");
	printf("	  quit : exit\n");
}

void do_list(struct sockaddr_in serveraddr)
{
	int sockfd, err;
	char buf[N];

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1)
	{
		perror("socket error");
		exit(1);
	}

	err = connect(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if(err == -1)
	{
		perror("connect error");
		exit(1);
	}
	
	buf[0] = 'l';
	write(sockfd, buf, N);

	while(read(sockfd, buf, N))
	{
		printf("%s\n", buf);
	}

	close(sockfd);
}

void do_get(struct sockaddr_in serveraddr, char fname[])
{
	int sockfd, fd, nbytes, err;
	char buf[N];

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1)
	{
		perror("socket error");
		exit(1);
	}

	err = connect(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if(err == -1)
	{
		perror("connect error");
		exit(1);
	}
	
	sprintf(buf, "g%s", fname);
	write(sockfd, buf, N);
	read(sockfd, buf, 1);
	if(buf[0] == 'N')
	{
		printf("open file %s from server failed\n", fname);
		close(sockfd);
		return;
	}
	
	fd = open(fname, O_WRONLY|O_CREAT|O_TRUNC,0666);
	if(fd < 0)
	{
		perror("open file failed");
		close(fd);
		return;
	}

	while(nbytes = read(sockfd, buf, N))
	{
		write(fd, buf, nbytes);
	}

	close(fd);
	close(sockfd);

}

void do_put(struct sockaddr_in serveraddr, char fname[])
{
	int sockfd, fd, nbytes, err;
	char buf[N];

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1)
	{
		perror("socket error");
		exit(1);
	}

	err = connect(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if(err == -1)
	{
		perror("connect error");
		exit(1);
	}
	
	sprintf(buf, "p%s", fname);
	write(sockfd, buf, N);
	read(sockfd, buf, 1);
	if(buf[0] == 'N')
	{
		printf("the file %s from server exist\n", fname);
		close(sockfd);
		return;
	}
	
	fd = open(fname, O_RDONLY);
	if(fd < 0)
	{
		perror("open file failed");
		close(fd);
		return;
	}

	while(nbytes = read(fd, buf, N))
	{
		write(sockfd, buf, nbytes);
	}
	close(fd);
	close(sockfd);
}

int main(int argc, char *argv[])
{
	int serverfd;
	struct sockaddr_in serveraddr;
	char command[N];

	if(argc < 3)
	{
		printf("usage:%s ip port\n", argv[0]);
		return -1;
	}

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(atoi(argv[2]));
	serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
	memset(serveraddr.sin_zero, 0, 8);

	while(1)
	{
		printf("client > ");
		fgets(command, N, stdin);
		command[strlen(command)-1] = '\0';
		if(strncmp(command, "help", 4) == 0)
		{
			do_help();
		}

		else if(strncmp(command, "list", 4) == 0)
		{
			do_list(serveraddr);
		}

		else if(strncmp(command, "get", 3) == 0)
		{
			do_get(serveraddr, command+4);
		}

		else if(strncmp(command, "put", 3) == 0)
		{
			do_put(serveraddr, command+4);
		}

		else if(strncmp(command, "quit", 4) == 0)
		{
			printf("bye\n");
			exit(0);
		}

		else
		{
			printf("invalid command %s\n", command);
			do_help();
		}

	}

	return 0;
}
