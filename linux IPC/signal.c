/*	写一段程序，使用系统调用fork( )创建两个子进程，再用系统调用signal( )让父进程捕捉键盘上来的中断信号（即按ctrl+c键），
	当捕捉到中断信号后，父进程用系统调用kill( )向两个子进程发出信号，子进程捕捉到父进程发来的信号后，分别输出下列信息后终止：  
		Child process 1 is killed by parent!
		Child process 2 is killed by parent!
	父进程等待两个子进程终止后，输出以下信息后终止：
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
