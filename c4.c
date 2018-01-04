#include <stdio.h>                                                                             
#include <time.h>
#include <stdlib.h>

//scnaf()
int main1()
{
		char arr[100];
		printf("输入字符串\n");

		scanf("%s",arr);

		printf("接收的字符串:[%s]\n",arr);

		return;
}

//gets()
int main2()
{
		char arr[100];
		printf("输入字符串\n");

		gets(arr);

		printf("接收的字符串:[%s]\n",arr);

		return;
}

//fgets()
int main3()
{
		char arr[100];
		printf("输入字符串\n");

		fgets(arr,13,stdin);

		printf("接收的字符串(size=13):[%s]\n",arr);

		return 0;
}

//puts()
int main4()
{
		char arr[100] = "hello world";
		puts(arr);
		return 0;
}

//fputs()
int main5()
{
		char arr[100] = "hello world";
		fputs(arr,stdout);
		return 0;
}

//strcpy()拷贝溢出情况
#include <string.h>
int main6()
{
		char dst[10] = {0};
		char src[11] = "helloworld";
		strcpy(dst,src);
		printf("dst string is: [%s]\n", dst);

		return 0;
}

//strcmp()
int main7()
{
		char arr1[10] = "helloa";
		char arr2[10] = "hellob";

		int ret = strcmp(arr1,arr2);

		printf("strcmp ret:[%d]\n",ret);

		return 0;
}

//strtok
int main8()
{
		char a[100] = "adc*fvcv*ebcy*hghbdfg*casdert"; 
		char *s = strtok(a, "*");
		while (s != NULL) 
		{ printf("%s\n", s); s = strtok(NULL, "*"); }

		return 0;
}

//mystrtok()
char* INDEX = NULL;
char *mystrtok(char *str, const char *delim)
{
		//extern INDEX;

		if(str != NULL) 
		{ 
				INDEX = str;
		}
		else if(INDEX != NULL)
		{
				str = INDEX;
		}
		else return NULL;

		if(delim == NULL) 
				return NULL;

		char* pos = str;
		//pos指向delim或指向字符串结尾则循环结束
		while(*pos != *delim && *pos != '\0')
				pos++;

		char* temp;
		if(*pos == *delim)
		{
				*pos = '\0';
				temp = INDEX;
				INDEX = pos+1; //全局指针指向下一阶段
				return temp;
		}
		else
		{
				temp = INDEX;
				INDEX = NULL;
				return temp;
		}
}

int main()
{
		char a[100] = "adc*fvcv*ebcy*hghbdfg*casdert"; 
		char *s = mystrtok(a, "*");
		while (s != NULL) 
		{ printf("%s\n", s); s = mystrtok(NULL, "*"); }

		return 0;
}
