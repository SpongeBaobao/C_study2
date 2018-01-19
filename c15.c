#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//1
//函数指针
/*
   void func(int,int);

   int main(){
//1
typedef void funtype(int,int);
funtype* f1 = func;
f1(1,1);
(*f1)(1,2);

//2
typedef void (*funcpntr)(int,int);
funcpntr f2 = func;
f2(2,1);
(*f2)(2,2);
printf("sizeof func pointer:%u\n",sizeof(funcpntr));

//3
void (*f3)(int,int) = func;
f3(3,1);
(*f3)(3,2);

return 0;
}

void func(int a, int b){
printf("func:a=%d--b=%d\n",a,b);

return;
}
*/

//2 函数指针数组
/*
   void func01(int a){
   printf("func01:%d\n",a);
   }
   void func02(int a){
   printf("func02:%d\n", a);
   }
   void func03(int a){
   printf("func03:%d\n", a);
   }

   void main()
   {
#if 0
void(*func_array[])(int) = { func01, func02, func03 }; //函数指针数组名: func_array(二级指针)
#else
void(*func_array[3])(int);
func_array[0] = func01;
func_array[1] = func02;
func_array[2] = func03;
#endif

for (int i = 0; i < 3; i ++){
func_array[i](10 + i);    //1
(*func_array[i])(10 + i); //2
}
}
*/

//3 callback
/*
   int plus(int a,int b){
   return a + b;
   }

   int minus(int a,int b){
   return a - b;
   }

#if 0
int caculator(int a,int b,int(*func)(int,int)){
return func(a, b);
}
#else
typedef int(*FUNC_POINTER)(int, int);
int caculator(int a, int b, FUNC_POINTER func){
return func(a, b);
}
#endif

int main()
{
int a = 10;
int b = 5;
int ret;

ret = caculator(a,b,plus);
printf("plus:%d+%d=%d\n",a,b,ret);

ret = caculator(a,b,minus);
printf("plus:%d+%d=%d\n",a,b,ret);

return 0;	
}
*/

//4递归函数
/*
   void func(int a){
   if(a == 1){
   printf("最底层函数\n");
   return;
   }

   printf("第%d层函数\n",a);

   func(a-1);

   return;
   }

   int main(){
   printf("开始调用递归函数\n");
   func(10);
   printf("递归函数调用结束\n");

   return 0;
   }
   */


//打印一个整数的各个位数
/*
   void func(int a){
   if(a == 0){
   return;
   }

   func(a/10);
   printf("%d\n",a%10);

   return;
   }

   int main(){
   int a = 8793;
   func(a);

   return;
   }
   */

//实现字符串的翻转
/*
void func(char* str, int i, int j){
		if(i >= j) return;

		func(str,i+1,j-1);

		char temp = str[i];
		str[i] = str[j];
		str[j] = temp;

		return;		
}

int main(){
		char buf[1024] = "hello world";
		printf("1:%s\n",buf);
		func(buf,0,strlen(buf)-1);
		printf("2:%s\n",buf);

		return 0;
}
*/


//宏定义
#include<sys/types.h> 
#include<sys/stat.h> 
#include<fcntl.h>

int main()
{
printf("file:%s\n", __FILE__);
printf("date:%s\n", __DATE__);
printf("time:%s\n", __TIME__);

int fd = open("./test.c",O_RDONLY);
if(fd == -1){
printf("open err,line:%d\n", __LINE__);
}

return 0;
}
