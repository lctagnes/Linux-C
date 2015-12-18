/*
利用文件IO的函数接口，实现一个拷贝文件功能的程序。*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

int main()
{
	int fdin, fdout;
	int readnum;
	char buf[512];
	memset(buf, 0, 512);

	if((fdin = open("file_in", O_RDONLY)) < 0)
	{
	printf("open file_in error!\n");
	exit(0);
	}
	if((fdout = open("file_out",O_RDWR|O_CREAT|O_APPEND, 0666)) < 0)
	{
	printf("open file_out error!\n");
	exit(0);
	}
	while(readnum = read(fdin, buf, 512))
	{
		write(fdout, buf, readnum);
	}
	close(fdin);
	close(fdout);
	return 0;
}
