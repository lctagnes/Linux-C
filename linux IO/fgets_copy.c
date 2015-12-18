/*文件拷贝
使用fgets这样的行IO方式，设计程序*/


#include<stdio.h>
#include<stdlib.h>
int main()
{
	char buf[10];
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
	while(fgets(buf,9,pin))
	{
		fputs(buf,pout);
	}
	fclose(pin);
	fclose(pout);
	return 0;
}
