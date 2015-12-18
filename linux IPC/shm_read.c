/*实现reader和writer两个进程通过共享内存交换数据。
	writer从标准输入读入字符串写入共享内存，reader把共享内存里的字符串打印到标准输出。
	reader和writer通过信号实现同步
*/

#include <stdio.h>
#include<stdlib.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>
#include<unistd.h>
#include<signal.h>
#include<errno.h>
#include<string.h>
#define BUFSIZE 1024

typedef struct
{
   pid_t pid_s;
   char sharebuf[BUFSIZE];
}share;

void fun(int signo)
{
   return;
}

int main()
{
   pid_t pid;
   int shmid;
   key_t key;
   share *shmaddr;

   if(key = ftok(".", 1) < 0)
   {
      perror("ftok failed");
      exit(1);
   }

   signal(SIGUSR1, fun);

  shmid = shmget(key, sizeof(share), IPC_CREAT|IPC_EXCL|0666);
  if(shmid < 0)

   {
      if(EEXIST == errno)
      {
	 shmid = shmget(key, sizeof(share), 0666);
	 shmaddr = (share *)shmat(shmid, NULL, 0);
	 pid = shmaddr->pid_s;
	 kill(pid, SIGUSR1);
	 shmaddr->pid_s = getpid();
      }
      else
      {	 
      perror("shmget error");
      exit(1);
      }
   }
   else
   {
      shmaddr = (share *)shmat(shmid, NULL, 0);

      if((int)shmaddr == -1)
      {
	 perror("shamt error");
	 exit(1);
      }

      shmaddr->pid_s = getpid();

      pause();


      pid = shmaddr->pid_s;
   }

      while(1)
      {
	 pause();
	 if(strcmp(shmaddr->sharebuf, "quit\n") == 0)
	    exit(0);
	 printf("read from shm:%s\n", shmaddr->sharebuf);

	 kill(pid, SIGUSR1);
      }

   return 0;
}
