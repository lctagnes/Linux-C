/*���㱾���ı�׼IO�����С
	����֤stdout���л��桢stderr�ǲ�����*/

#include<stdio.h>
void main()
{
	fprintf(stdout, "1======1");
	fprintf(stderr, "2======2");
	fprintf(stdout, "3======3\n");
	fprintf(stderr, "4======4\n");
}
