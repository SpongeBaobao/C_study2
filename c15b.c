#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _PERSON{
		char name[32];
		int age;
		long ID;
		float score;
}person;

/*
   int main(){
   person p1 = {"mike",18,110123,89.5};

//1 结构结构体类型,获取各个成员的长度信息
//数据长度用于存储数据,类型长度用于设置索引步长
int typelen1 = sizeof(char);
int datalen1 = sizeof(p1.name);
int typelen2 = sizeof(int);
int datalen2 = sizeof(p1.age);
int typelen3 = sizeof(long);
int datalen3 = sizeof(p1.ID);
int typelen4 = sizeof(float);
int datalen4 = sizeof(p1.score);

//2 对第一个成员直接创建空间进行存储
int len = 0;
char* p0 = (char*)malloc(datalen1);
memcpy(p0,p1.name,datalen1);
len = datalen1;  //已经存储数据所占用的空间长度

//3 第二个成员
while(len % typelen2 != 0){
len++;
}
//len%typelen == 0
p0 = realloc(p0,len+datalen2);
memcpy(p0+len,&(p1.age),datalen2);
len += datalen2;


//4 第三个成员
while(len % typelen3 != 0){
len++;
}
//len%typelen == 0
p0 = realloc(p0,len+datalen3);
memcpy(p0+len,&(p1.ID),datalen3);
len += datalen3;

//5 第4个成员
while(len % typelen4 != 0){
len++;
}
//len%typelen == 0
p0 = realloc(p0,len+datalen4);
memcpy(p0+len,&(p1.score),datalen4);
len += datalen4;

//6 比较验证
int ret = memcmp(&p1,p0,sizeof(p1));
if(ret == 0){
printf("equal\n");
}
else{
printf("unequal\n");
}

printf("size of struct person is:%u\n",sizeof(person));
printf("size of struct person is:%u\n",len);

return 0;
}
*/

typedef struct _MEMLEN{
		int typelen;
		int datalen;
		char* pmem;
}meminfo;

int main(){
		person p1 = {"mike",18,110123,89.5};

		//1 结构结构体类型,获取各个成员的长度信息
		//数据长度用于存储数据,类型长度用于设置索引步长
		int typelen1 = sizeof(char);
		int datalen1 = sizeof(p1.name);
		char* pm1 = (char*)p1.name;
		int typelen2 = sizeof(int);
		int datalen2 = sizeof(p1.age);
		char* pm2 = (char*)&p1.age;
		int typelen3 = sizeof(long);
		int datalen3 = sizeof(p1.ID);
		char* pm3 = (char*)&p1.ID;
		int typelen4 = sizeof(float);
		int datalen4 = sizeof(p1.score);
		char* pm4 = (char*)&p1.score;

		int mem_num = 4;
		//2 组装结构体成员长度信息
		struct _MEMLEN  ml[4] =   \
		{{typelen1,datalen1,pm1}, \
				{typelen2,datalen2,pm2},  \
				{typelen3,datalen3,pm3},  \
				{typelen4,datalen4,pm4} };

		//3 按照内存对齐原则进行结构体成员数据存储
		char* p0 = NULL;
		int len = 0;
		int i;
		for(i = 0; i < mem_num; i++){
				while(len % (ml[i]).typelen != 0){
						len++;
				}
				//len == 已经存储的数据长度 + 为对齐所空余的长度 

				p0 = (char*)realloc(p0,len+(ml[i]).datalen); //?
				memcpy(p0+len,(ml[i]).pmem,(ml[i]).datalen);
				len += (ml[i]).datalen;  //已经存储数据所占用的空间长度
		}

		//4 结果检查
		int ret = memcmp(&p1,p0,sizeof(p1));
		if(ret == 0){
				printf("equal\n");
		}
		else{
				printf("unequal\n");
		}

		printf("size of struct person is:%u\n",sizeof(person));
		printf("size of struct person is:%u\n",len);

		return 0;
}
