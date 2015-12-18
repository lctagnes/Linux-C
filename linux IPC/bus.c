/*
˾����ƱԱ����
	�����ӽ��̴�����ƱԱ�������̴���˾�� ��ͬ���������£�

	��ƱԱ��׽SIGINT(������)����SIGUSR1��˾����˾����ӡ(��let��s gogogo��)
	��ƱԱ��׽SIGQUIT(����ͣ��)����SIGUSR2��˾����˾����ӡ(��stop the bus��)
	˾����׽SIGTSTP(��������վ)����SIGUSR1����ƱԱ����ƱԱ��ӡ(��please get off the bus��)

*/

#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/wait.h>

   pid_t pid;
void seller_INT()
{
   kill(getppid(), SIGUSR1);
}

void seller_QUIT()
{
   kill(getppid(), SIGUSR2);
}

void seller_usr1()
{
   printf("please get off the bus!\n");
}

void driver_usr1()
{
   printf("let's go go go!\n");
}

void driver_usr2()
{
   printf("stop the bus!\n");
}

void driver_tstp()
{
   kill(pid, SIGUSR1);
}
int main()
{

   pid = fork();
   if(pid == -1)
   {
      perror("fork error");
      exit(1);
   }
   else if(pid == 0)
   {
      signal(SIGINT, seller_INT);
      signal(SIGQUIT, seller_QUIT);
      signal(SIGTSTP, SIG_IGN);
      signal(SIGUSR1, seller_usr1);
      while(1);
   }
   else
   {
      signal(SIGINT, SIG_IGN);
      signal(SIGUSR1, driver_usr1);
      signal(SIGQUIT, SIG_IGN);
      signal(SIGUSR2, driver_usr2);
      signal(SIGTSTP, driver_tstp);
      while(1);
   }

   return 0;
}
