#include <stdio.h>
#include <stdlib.h>

/*
   int main()
   {
   int arr[10] = {0,1,2,3,4,5,6,7,8,9};
   printf("[arr]=%p\n",arr);
   printf("[&arr[0]]=%p\n",&arr[0]);
   printf("[sizeof(arr)]=%u\n",sizeof(arr));
   printf("[sizeof(arr[0])]=%u\n",sizeof(arr[0]));

   return 0;
   }
   */

//冒泡排序(由小到大)
/*
int main()
{
		int arr[10] = {4,1,8,3,9,5,6,10,7,9};
		int i,j,flag;
		int num = sizeof(arr)/sizeof(arr[0]);

		printf("排序前:");
		for(i = 0; i < num; i++)
		{
				printf("%d\t",arr[i]);
		}
		printf("\n");

		for(i = 0; i < num-1; i++)
		{
				flag = 0;
				for(j = 0; j < num-i-1; j++)
				{
						if(arr[j] > arr[j+1])
						{
								int temp = arr[j];
								arr[j] = arr[j+1];
								arr[j+1] = temp;
								flag = 1;
						}
				}
				if(flag == 0)
						break;
		}

		printf("排序后:");
		for(i = 0; i < num; i++)
		{
				printf("%d\t",arr[i]);
		}
		printf("\n");

		return 0;
}
*/

//二维数组
int main()
{
int a[3][4] = { 1, 2, 3, 4 , 5, 6, 7, 8, 9, 10, 11, 12 };
printf("[a] = %p\n", a); 
printf("[a[0]] = %p\n", a[0]);
printf("[&a[0][0]] = %p\n", &a[0][0]);

printf("sizeof(a) = %d\n", sizeof(a));
printf("sizeof(a[0]) = %d\n", sizeof(a[0]) );
printf("sizeof(a[0][0]) = %d\n", sizeof(a[0][0]));

printf("行数 = %d\n", sizeof(a) / sizeof(a[0]));
printf("列数 = %d\n", sizeof(a[0]) / sizeof(a[0][0]));
printf("元素数 = %d\n", sizeof(a) / sizeof(a[0][0]));

return 0;
}
