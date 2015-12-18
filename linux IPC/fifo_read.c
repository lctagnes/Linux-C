#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
/*���������ܵ��Ĵ����Ͷ�д��ʽ
	1.���������ܵ�
	2.������ִ��ѭ���ȴ����ݱ�д�뵽�ܵ��в��������ܵ�
	3.�������ܵ���д���ݵ����ܵ�*/

#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<fcntl.h>
#include<errno.h>
#define FIFO_PATH "myfifo"

int main()
{
   int fd, num_r;
   char readbuf[255] = {0};
   if(mkfifo(FIFO_PATH, 0666) < 0 && errno != EEXIST)
   {
      perror("mkfifo failed");
      exit(0);
   }
   else
   {
      printf("create fifo sucess\n");
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
     num_r =  read(fd, readbuf, 255);
     if(num_r > 0)
	printf("the read length is %d, readbuf is %s\n", num_r, readbuf);
	else
	   printf("read failed\n");
   }
   }

   close(fd);

   return 0;


}
