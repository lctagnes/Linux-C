/*
ʵ��һ��ls -l�Ĺ��ܣ�����������ʾ�ļ�����Ϣ�������ļ���״̬��Ϣ�������ļ��Ĵ�С��Ȩ�ޡ�*/
	
#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<dirent.h>

//��ȡ�ļ�Ȩ��
int get_rwx(char *fp)
{
   int n;
   struct stat buf;
   
   if(stat(fp, &buf) == -1)
      perror("stat error\n");
   
   for(n = 8; n >= 0; n--)
   {
      if(buf.st_mode & (1 << n))
      {
         switch(n % 3)
         {
            case 2:printf("r");break;
            case 1:printf("w");break;
            case 0:printf("x");break;
            default:printf("-");break;
         }
      } 
   }

   return 0;

}

//��ȡ�ļ�����
int get_mode(char *fp)
{
   struct stat buf;

   if((lstat(fp, &buf)) < 0)
      perror("lstat error\n");

   switch(S_IFMT & buf.st_mode)
   {
      case S_IFREG: printf("-");break;
      case S_IFDIR: printf("d");break;
      case S_IFCHR: printf("c");break;
      case S_IFBLK: printf("b");break;
      case S_IFIFO: printf("f");break;
      case S_IFLNK: printf("l");break;
      case S_IFSOCK:printf("s");break;
      defalut: printf("UNKOWN");break;
   }

   return 0;
}

//��ȡ�ļ���С
long get_size(char *fp)
{
   long size;
   struct stat buf;

   if((stat(fp, &buf)) == -1)
      perror("stat get_mode error\n");

   size = buf.st_size;


   return size;

}

//��ȡӲ������
int get_nlink(char *fp)

{
   struct stat buf;

   if((stat(fp, &buf)) == -1)
      perror("stat get_nlink error\n");

   printf("%-5d", buf.st_nlink);

   return 0;

}

//��ȡ�޸�ʱ��
int get_time(char *fp, char *ch)
{
   struct stat buf;

   if((stat(fp, &buf)) == -1)
      perror("stat get_time error\n");

   strcpy(ch, ctime(&buf.st_mtime));
   ch[strlen(ch) - 1] = '\0';

   return 0;
}

//��ȡ�ļ�����Ϣ
struct dirent **get_scan(int *n)
{
   struct dirent **namelist;

   *n = scandir(".", &namelist, 0, alphasort);

   if((*n) < 0)
      perror("not found!\n");

   return namelist;
}

//ʵ��ls����
int ls()
{
   struct dirent **namelist;
   int i, n;

   namelist = get_scan(&n);

   for(i = 0; i < n; i++)
   {
      if(namelist[i] -> d_name[0] != '.')
	 printf("  %s", namelist[i] -> d_name);
	free(namelist[i]);
   }

   printf("\n");
   free(namelist);

   return 0;

}

//ʵ��ls -l����
int ls_l()
{
   struct dirent **namelist;
   int i, n;
   long sum_size, size;
   char ch[40] = {0};

   namelist = get_scan(&n);

   for(i = 0; i < n; i++)
   {
      if(namelist[i] -> d_name[0] != '.')
      {
	 size = get_size(namelist[i] -> d_name);
	 sum_size += size;
      }
   }
   printf("total %ld\n", sum_size);

   for(i = 0; i < n; i++)
   {
      if(namelist[i] -> d_name[0] != '.')
      {
	get_rwx(namelist[i] -> d_name);
	get_mode(namelist[i] -> d_name);
	get_nlink(namelist[i] -> d_name);
	printf("%-5ld", size);
	get_time(namelist[i] -> d_name, ch);
	printf(" %-s", ch);
	printf(" %-s", namelist[i]->d_name);
	printf("\n");
      }

      free(namelist[i]);
   }

   free(namelist);

   return 0;

}

int main(int argc, char *argv[])
{
   if(argc == 1)
      ls();
   else if(argc == 2)
      ls_l();

   return 0;
}
