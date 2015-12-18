/*��֤�ļ�����ı�׼IO��ȫ����IO,�����׼IO�л������Ĵ�С*/


#include<stdio.h>

int stream_buf(FILE *fp)
{
   if(fp->_flags & _IO_UNBUFFERED)
      printf("tne IO type is unbuffered\n");
   else if(fp->_flags & _IO_LINE_BUF)
      printf("the IO type is line buf\n");
   else
      printf("the IO type is full buf\n");
   printf("the IO size is : %d\n", fp->_IO_buf_end - fp->_IO_buf_base);

   return 0;
}

int main()
{
   FILE *fp = NULL;
   printf("stdin:==================\n\n");
   stream_buf(stdin);

   printf("\n\nstdout:=================\n\n");
   stream_buf(stdout);

   printf("\n\nstderr:=================\n\n");
   stream_buf(stderr);

   printf("\n\nfile:===================\n\n");
   if((fp = fopen("test.txt", "w+")) == NULL)
      perror("open file failed");

   printf("\nbefore write:\n\n");
   stream_buf(fp);

   fputc('1', fp);

   printf("\nafter write:\n\n");
   stream_buf(fp);

   return 0;

}
