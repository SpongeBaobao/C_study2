#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//指针数组
/*
   void main()
   {
   char *p[] = { "aaaaa", "bbbbb", "ccccc","ddddd" };

   for(int i = 0; i<(sizeof(p)/sizeof(p[0]));i++){
   printf("--%s--\n",p[i]);
   }

   return;	
   }
   */

//一元位操作符
/*
   void main(){
//unsigned char a = 2;
//unsigned char b = ~a;
//printf("a=%u\nb=%u\n",a,b);

//补码实现
//int a = 2;
//int b = ~a + 1;
//printf("a=%d\nb=%d\n",a,b);

//&
//int a = 0xA;
//int b = 0x5;
//printf("a&b=%d\n",a&b);

//|
//int a = 0xA;
//int b = 0x5;
//printf("a|b=%x\n",a|b);

//^
int a = 0xA;
int b = 0x5;
printf("a|b=%x\n",a^b);
printf("a|b=%x\n",a^a);

return;
}
*/

//两个变量的值得互换
/*
   int main()
   {
   int a = 100;
   int b = 1;
   printf("交换前:a=%d\tb=%d\n",a,b);

//1
int temp = a;
a = b;
b = temp;
printf("第一次交换:a=%d\tb=%d\n",a,b);

//2
a = b ^ a;
b = a ^ b;
a = b ^ a;
printf("第二次交换:a=%d\tb=%d\n",a,b);

return 0;
}
*/

//移位操作
/*
   int main()
   {
//int a = 4;
//int b = a << 1;
//b >>= 2;

short int a = -10;
short int b = a >> 1;
printf("a=%hd--b=%hd\n",a,b);


}
*/

//数组与指针
/*
   void main()
   {
   int arr[10];
   int *p = arr;
   printf("sizeof(p):%d\n", sizeof(p)); //4

   printf("sizeof(arr):%d\n", sizeof(arr)); //40
   printf("arr = %p\n",arr);

   printf("sizeof(&arr):%d\n", sizeof(&arr)); //4
   printf("&arr = %p\n",&arr);

   return;	
   }
   */

//数组下标
/*
   int main(){
   int arr[] = { 5, 3, 6, 8, 2, 9 };
   int *p = arr + 2;
   printf("*p = %d--%d\n", *p,arr[2]);
   printf("*p = %d\n", p[-1]);

   return 0;
   }
   */

//
/*
   void main(){
   int* p;
   printf("%p\n",p);
   p++;
   printf("%p\n",p);

   return;
   }
   */

//二维数组
/*
   void main()
   {
   int arr[3][3] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
   printf("&arr[0][0]=%p\n",&arr[0][0]);
   printf("arr[0]=%p\n",arr[0]);
   printf("arr=%p\n",arr);
   printf("&arr=%p\n",&arr);

   return;	
   }
   */

//数组类型
/*
   void main()
   {
   int arr[10] = {1,2,3,4,5,6,7,8,9,10};

   typedef int(ArrayType)[10]; //定义数组类型
   ArrayType myarr;            //定义数组
   for (int i = 0; i < 10;i++){
   myarr[i]=arr[i];
   }
   for (int i = 0; i < 10;i++){
   printf("%d ",myarr[i]);
   }


   ArrayType* pArr = &arr;     //定义数组指针,并初始化
   for (int i = 0; i < 10;i++){
   printf("%d ",(*pArr)[i]);
   }
   printf("\n");

   return;	
   }
   */

//
/*
void main()
{
		int arr[10];

		typedef int(*ArrayType)[10];  //定义数组指针类型
		ArrayType pArr = &arr;

		for (int i = 0; i < 10; i++){
				(*pArr)[i] = i + 1;
		}

		for (int i = 0; i < 10; i++){
				printf("%d ", (*pArr)[i]);
		}
		printf("\n");

		return;	
}
