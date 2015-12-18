/*掌握有名管道的创建和读写方式
	1.创建有名管道
	2.本进程执行循环等待数据被写入到管道中并读有名管道
	3.打开有名管道并写数据到名管道*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<fcntl.h>
#include<errno.h>
#define FIFO_PATH "myfifo"

int main()
{
   int fd, num_wr;
   char writebuf[] = "I want to write something into myfifo! That's what I want to write,haha!\n";
   if(mkfifo(FIFO_PATH, 0666) < 0 && errno != EEXIST)
   {
      perror("mkfifo failed");
      exit(0);
   }

   fd = open(FIFO_PATH, O_RDWR|O_CREAT, 0666 );
   if(fd == -1)
   {
      perror("open failed");
      exit(0);
   }
   if(fd > 0)
   {

   while(1)
   {
     num_wr =  write(fd, writebuf, strlen(writebuf));
     if(num_wr > 0)
	printf("the write length is %d, writebuf is %s\n", num_wr, writebuf);
	sleep(2);
   }
   }

   close(fd);

   return 0;


}
