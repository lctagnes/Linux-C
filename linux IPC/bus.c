/*
司机售票员问题
	创建子进程代表售票员，父进程代表司机 ，同步过程如下：

	售票员捕捉SIGINT(代表开车)，发SIGUSR1给司机，司机打印(“let’s gogogo”)
	售票员捕捉SIGQUIT(代表停车)，发SIGUSR2给司机，司机打印(“stop the bus”)
	司机捕捉SIGTSTP(代表车到总站)，发SIGUSR1给售票员，售票员打印(“please get off the bus”)

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
