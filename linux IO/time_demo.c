/*�ļ���ӷ�ʽ:
��̶�дһ���ļ�test.txt��ÿ��1�����ļ���д��һ������,
�´�����������д�ļ�ʱ����׷�ӵ�ԭ�ļ�֮�󣬲�������ܹ������ϴε����*/


#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<time.h>

int main()
{
   struct tm *now;
   time_t t;
   char buf[100];
   char c;
   int line = 1;
   FILE *fp = NULL;

   memset(buf, 0, 100);

   if((fp = fopen("test.txt", "a+")) == NULL)
   {
      perror("open error\n");
      exit(0);
   }

   while(fgets(buf, 100, fp))
   {
      if(buf[strlen(buf) - 1] == '\n')
      {
	 line++;
      }
   }

   while(1)
   {
      t = time(0);
      now = localtime(&t);

      sprintf(buf, "%d, %d-%d-%d %d:%d:%d\n", line++, now->tm_year + 1900, now->tm_mon + 1, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec );

      fwrite(buf, sizeof(char), strlen(buf), fp);

      fflush(fp);
      sleep(1);
   }

   fcolse(fp);

   return 0;
}
