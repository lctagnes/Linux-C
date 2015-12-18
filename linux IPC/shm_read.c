/*ʵ��reader��writer��������ͨ�������ڴ潻�����ݡ�
	writer�ӱ�׼��������ַ���д�빲���ڴ棬reader�ѹ����ڴ�����ַ�����ӡ����׼�����
	reader��writerͨ���ź�ʵ��ͬ��
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
