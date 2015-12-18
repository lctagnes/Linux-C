/*	дһ�γ���ʹ��ϵͳ����fork( )���������ӽ��̣�����ϵͳ����signal( )�ø����̲�׽�����������ж��źţ�����ctrl+c������
	����׽���ж��źź󣬸�������ϵͳ����kill( )�������ӽ��̷����źţ��ӽ��̲�׽�������̷������źź󣬷ֱ����������Ϣ����ֹ��  
		Child process 1 is killed by parent!
		Child process 2 is killed by parent!
	�����̵ȴ������ӽ�����ֹ�����������Ϣ����ֹ��
        Parent process exit!*/

#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/wait.h>

int wait_mark;

void waiting()
{
   while(wait_mark);
}

void stop()
{
   wait_mark = 0;
}

int main()
{
   pid_t pid1, pid2;

   if(pid1 = fork())
   {
      if(pid2 = fork())
      {
	 signal(SIGINT, stop);
	 wait_mark = 1;
	 waiting();
	 sleep(1);
	 kill(pid2, SIGUSR2);
	 sleep(1);
	 kill(pid1, SIGUSR1);
	 wait(NULL);
	 wait(NULL);
	 printf("parent process is exit!\n");
	 exit(0);
      }
      else
      {
	 signal(SIGINT,	stop);
	 wait_mark = 1;
	 waiting();
	 signal(SIGUSR2, stop);
	 wait_mark = 1;
	 waiting();
	 printf("Child process 2 is killed by parent!\n");
	 exit(0);
      }
   }
   else
   {
      signal(SIGINT, stop);
      wait_mark = 1;
      waiting();
      signal(SIGUSR1, stop);
      wait_mark = 1;
      waiting();
      printf("Child process 1 is killed by parent!\n");
      exit(0);
   }

   return 0;
}
