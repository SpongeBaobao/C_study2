#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
   int main()
   {
   return 0;
   }
   */

//1
/*
   int* func(){
   int a = 100;
   static int b = 200;
//		int* p = &a;
int* p = &b;

return p;
}

int main()
{
int* p = func();
printf("A:%p--%d\n",p,*p);
printf("B:%p--%d\n",p,*p);
return 0;
}
*/

//2指针的步长
/*
   int main()
   {
   int arr[3] = {0};
   arr[0] = 0xaabbccdd;
   arr[1] = 1;
   arr[2] = 2;

   int* p = arr;
   for(int i = 0; i<3; i++){
   printf("%d: %d\n",i,*(p+i));
   }

   char* q = NULL;
   q = arr;
   for(int j=0; j<4;j++){
   printf("%d: %x\n",j,*(q+j));
   }

   return 0;
   }
   */

//3
/*
   void main()
   {
   int intArray[] = { 10, 20, 30, 40 };

   printf("intArray[] = %d\n", intArray);
   printf("intArray[] = %d\n", intArray + 1);
   printf("intArray[] = %d\n", *(intArray + 1));
   printf("intArray[] = %d\n", intArray);
   printf("intArray[] = %d\n", &intArray + 1);
   printf("intArray[] = %d\n", &intArray);

   char charArray[] = { 'a', 'b', 'c', 'd' };
   printf("charArray[] = %c\n", *(charArray + 1));
   }
   */

//字符串和数组
/*
   int main()
   {
//char str3[100] = "hello";
//char str3[] = "hel\0lo";
char str3[] = "hello\012world";
// \0==>char
// \012 ==> char
printf("%s\n",str3);
printf("sizeof str:%d\n",sizeof(str3));
printf("strlen str:%d\n",strlen(str3));

return 0;
}
*/

//string copy
/*
void copy_string(char* dest,const char* source ){
		int i =0;
		while(*(source+i)!='\0')
		{
				*(dest+i) = *(source+i);
				i++;
		}

		*(dest+i) = '\0';
}

int main()
{
		char src[20] = "helloworld";
		char dest[20];
		copy_string(dest,src);
		printf("src:%s\n",src);
		printf("dest:%s\n",dest);

		return 0;
}
*/

//拼接字符串
/*
int main()
{
		char buf[1024];
		memset(buf, 0, 1024);
		char str1[] = "hello";
		char str2[] = "world\n";
		int len = sprintf(buf,"%s %s",str1,str2);
		printf("buf:%s len:%d\n", buf,len);
		int num = 100;
		//sprintf(buf, "%d", num);
		//sprintf(buf, "%8d", num);
		//sprintf(buf, "%-8d", num);
		//sprintf(buf, "0x%x", num);
		sprintf(buf, "0%o", num);
		printf("buf:[%s]\n", buf); //'1' '0' '0'

		printf("strlen(buf):%d\n",strlen(buf));

		return 0;
}
*/


//sprintf
/*
int main(){
char buf[20] = {0};
int ret = sprintf(buf,"a%ccd%cf",'B','E');
printf("%d--%s\n",ret,buf);

char x,y;
int ret2 = sscanf(buf,"a%ccd%cf",&x,&y);
printf("%d--%c--%c\n",ret2,x,y);

return 0;
}
*/

//sscanf
/*
int main(){
char buf[20] = {0};
//sscanf("123456aaaa", "%*d%s", buf); //buf:aaaa
//sscanf("123456aaaa", "%7s", buf); //buf:123456a
//sscanf("abcd7efg123456", "%[a-z]", buf); //buf:abcd
//sscanf("abcdefg123456", "%[aBc]", buf); //buf:a
//sscanf("bcdefag123456", "%[^a]", buf); //buf:bcdef
//sscanf("123456ABCDbcdefag", "%[^a-z]", buf); //buf:123456ABCD


printf("buf:%s\n",buf);

return 0;
}
*/

//scanf
/*
int main(){
int a,b,c;
scanf("%1d%2d%3d", &a, &b, &c);
printf("%d--%d--%d\n",a,b,c);
//122333
//1--22--333
return 0;
}
*/


//解析邮箱名称
/*
int main()
{
char email[50] = "helloworld@yahoo.cn";
char name1[20] = {0};
char name2[20] = {0};

//1
//sscanf(email,"%[^@]",name1);
//sscanf(email+strlen(name1)+1,"%[^.]",name2);

//2
sscanf(strstr(email,"@")+1,"%[^.]",name2);
strcpy(name1,strtok(email,"@")); //修改了原数据

printf("name1:%s\nname2:%s\n",name1,name2);

return 0;
}
*/


//123abcd$myname@000qwe
/*
int main()
{
char buf[50] = "123abcd$myname@000qwe";
char ret1[50] = {0};
char ret2[50] = {0};

sscanf(buf,"%[^@]",ret1); //ret1=123abcd$myname
strcpy(ret2,strstr(ret1,"$")+1);

printf("--%s--\n",ret2);

return;
}
*/

//指针作为返回值
/*
char *get_str()
{
		static int a = 999;
		static int b = 888;
		char str[] = "abbbbbbbbbb"; //栈区，
		printf("[get_str]str = %s\n", str);
		return str;
}

int main()
{
		char* p = get_str();

		printf("[main]str = %s\n", p);
		printf("[main]str = %s\n", p);

		return 0;
}
*/


