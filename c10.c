#include <stdio.h>

//1
/*
int p;
int q = 20;

void func(){
static a;
static b = 100;
char* p = "hell";

return;
}

int main(){
int a = 10;
int b = 20;
int c = a + b;
char* p = "hell";

printf("c=%d\n",c);

return 0;
}
*/

//小端对齐
void main(){
		int num = 0xaabbccdd;
		unsigned char* p = &num;

		printf("%x\n",*p);
		printf("%x\n", *(p + 1));
		printf("%x\n", *(p + 2));
		printf("%x\n", *(p + 3));
}
