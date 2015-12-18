/*��д����ʵ�ֽ��̵������ܵ�ͨ�š�
	��ϵͳ����pipe( )����һ�ܵ��������ӽ���P1��P2�ֱ���ܵ���дһ�仰��
		Child 1 is sending a message!
		Child 2 is sending a message!
	�����̴ӹܵ��ж������������ӽ��̵���Ϣ����ʾ��Ҫ���Ƚ���P1����P2����*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

int main()
{
   pid_t pid1, pid2;
   int fd[2];
   char writemesg[50];
   char readmesg[50];

   if(pipe(fd) == -1)
   {
      perror("create pipe failed");
      exit(1);
   }
   
   pid1 = fork();
   if(pid1 == -1)
   {
      perror("pid1 fork failed");
      exit(1);
   }
   else if(pid1 == 0)
   {
      lockf(fd[1], 1, 0);
      sprintf(writemesg, "Child1 is sending a message!\n");
      write(fd[1], writemesg, 50);
      sleep(3);
      lockf(fd[1], 0, 0);
      exit(0);
   }
   else
   {
      pid2 = fork();
      if(pid2 == -1)
      {
	perror("pid2 fork error");
	exit(1);
      }
      else if(pid2 == 0)
      {
        lockf(fd[1], 1, 0);
        sprintf(writemesg, "Child2 is sending a message!\n");
	write(fd[1], writemesg, 50);
	sleep(3);
        lockf(fd[1], 0, 0);
        exit(0);
      }
      else
      {
	 wait(0);
	 read(fd[0], readmesg, 50);
	 printf("%s\n",readmesg);
	 wait(0);
	 read(fd[0], readmesg, 50);
	 printf("%s\n",readmesg);
	 exit(0);
      }
   }

   return 0;

}
