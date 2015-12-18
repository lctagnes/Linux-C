/*计算本机的标准IO缓存大小
	请验证stdout是行缓存、stderr是不缓存*/

#include<stdio.h>
void main()
{
	fprintf(stdout, "1======1");
	fprintf(stderr, "2======2");
	fprintf(stdout, "3======3\n");
	fprintf(stderr, "4======4\n");
}
