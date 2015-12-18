/*文件拷贝
使用fread这样的块IO方式，设计程序*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int main()
{
	FILE *fpin, *fpout;
	int readnum;
	char buf[512];
	memset(buf, 0, 512);
	if((fpin = fopen("file_in", "r")) == NULL)
	{
	printf("open file_in error!\n");
	exit(0);
	}
	if((fpout = fopen("file_out","w+")) == NULL)
	{
	printf("open file_out error!\n");
	exit(0);
	}
	while(readnum = fread(buf, sizeof(char), 512, fpin))
	{
		fwrite(buf, sizeof(char), readnum, fpout);
	}
	fclose(fpin);
	fclose(fpout);
	return 0;
}
