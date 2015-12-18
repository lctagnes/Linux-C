/*文件拷贝
使用fgetc这样的字符IO方式，设计程序*/


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int main()
{
	char buf;
	FILE *pin, *pout;
	if((pin = fopen("file.in","r")) == NULL)
	{
	printf("open file.in error!\n");
	exit(0);
	}
	if((pout = fopen("file.out","w+")) == NULL)
	{
	printf("open file.out error!\n");
	exit(0);
	}
	while((buf = fgetc(pin))!=EOF)
	{
		fputc(buf,pout);
	}
	fclose(pin);
	fclose(pout);
}
