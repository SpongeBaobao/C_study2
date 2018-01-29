#include <sys/types.h>
#include <sys/stat.h> 
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//1 创建线程
/*
   void* thrfunc(void* arg)
   {
   printf("In thrfunc:thread%d--TID=%lu\n",(int)arg,pthread_self());
   return NULL;
   }

   int main()
   {
   int i;
   int ret;
   pthread_t tid[5];

   for(i = 0; i < 5; i++){

   ret = pthread_create(&tid[i],NULL,thrfunc,(void*)(i+1));
   if(ret == 0){
   printf("create thread%d ok:PID=%d--TID=%ld\n",i+1,getpid(),tid[i]);
   }
   }

   sleep(2);
   printf("main:PID=%d--TID=%lu\n",getpid(),pthread_self());

   return 0;
   }
   */

//2 线程共享全局变量
/*
   int a = 100; //全局变量
   void* thrfunc(void* arg)
   {
   printf("In thrfunc:thread%d--TID=%lu--INFO=%d\n",(int)arg,pthread_self(),a);
   return NULL;
   }

   int main()
   {
   int ret;
   pthread_t tid[2];

   ret = pthread_create(&tid[0],NULL,thrfunc,(void*)1);
   if(ret == 0){
   printf("create thread1 ok\n");
   }

   sleep(1);
   a = 200;
   ret = pthread_create(&tid[1],NULL,thrfunc,(void*)2);
   if(ret == 0){
   printf("create thread2 ok\n");
   }
   sleep(1);
   printf("main:PID=%d--TID=%lu--INFO=%d\n",getpid(),pthread_self(),a);

   return 0;
   }
   */

//3 线程退出和回收
/*
   int a = 0; //全局变量
   void* thrfunc(void* arg)
   {
   printf("In thrfunc:TID=%lu\n",pthread_self());
   a++;
   pthread_exit((void*)&a);
//return NULL;
}

int main()
{
int ret;
pthread_t tid;
ret = pthread_create(&tid,NULL,thrfunc,(void*)1);
if(ret == 0){
printf("create thread1 ok\n");
}

printf("main:PID=%d--TID=%lu\n",getpid(),pthread_self());

//回收子线程
int* p = NULL;
ret = pthread_join(tid,(void*)&p);
if(ret == 0){
printf("子线程回收成功:%d\n",*p);
}

return 0;
}
*/

//4 主控线程提前退出
/*
   void* thrfunc(void* arg){
   printf("In thread:%lu\n",pthread_self());
   while(1){
   printf("thread living\n");
   sleep(1);
   }
   }

   int main(){
   pthread_t tid;
   int ret = pthread_create(&tid,NULL,thrfunc,NULL);
   if(ret == 0){
   printf("pthread create ok\n");
   }

//sleep(1);
pthread_exit(NULL);
}
*/

//5
int a = 0;
void* thrfunc(void* arg)
{
		printf("In thrfunc:thread%d--TID=%lu\n",(int)arg,pthread_self());
		a++;
		pthread_exit((void*)&a);
}

int main()
{
		int i;
		int ret;
		int *p = NULL;  //回收子线程返回值

		pthread_t tid[5];

		for(i = 0; i < 5; i++){

				ret = pthread_create(&tid[i],NULL,thrfunc,(void*)(i+1));
				if(ret == 0){
						printf("create thread%d ok\n",i+1);
						if(i == 4){
								//最后一个线程设置为分离形式
								ret = pthread_detach(tid[i]);
								if(ret == 0){
										printf("thread%d set deach ok\n",i+1);
								}
						}
				}

				ret = pthread_join(tid[i],(void**)&p);
				if(ret == 0){
						printf("thread%d join ok:rvalue=%d--extern a=%d\n",i+1,*p,a);
				}
				else{
						printf("thread%d join error:%s\n",i+1,strerror(ret));
				}
		}

		sleep(2);
		printf("main thread:PID=%d--TID=%lu\n",getpid(),pthread_self());
		pthread_exit(NULL);
		//return 0;
}
/*
   create thread2 ok
   In thrfunc:thread2--TID=3075705664
   thread2 join ok:rvalue=2--extern a=2
   */

//6 主动杀死线程
/*
   void* thrfunc(void* arg)
   {
   printf("In thrfunc:TID=%lu\n",pthread_self());
   sleep(2);

//创建回收点动作
//char buf[1024] = "hello thread\n";
//write(STDOUT_FILENO,buf,strlen(buf));

while(1){
sleep(1);
}

pthread_exit(NULL);
}

int main()
{
pthread_t tid;
int ret = pthread_create(&tid,NULL,thrfunc,NULL);
if(ret == 0){
printf("mainthr create thread ok:mainthid=%lu--pid=%d\n",pthread_self(),getpid());
}

ret = pthread_cancel(tid);
if(ret == 0){
printf("thread cancel\n");
}


void* retvalue = NULL;
ret = pthread_join(tid,&retvalue);
if(ret == 0){
//printf("thread join ok: value=%d--PTHREAD_CANCELED=%d\n",*pvalue,*((int*)PTHREAD_CANCELED));
printf("thread join ok: value=%p\n",retvalue);
//thread join ok: value=0xffffffff
}

while(1){
;
}

pthread_exit(NULL);
}
*/

//7
void* thrfunc(void* arg)
{
		printf("In thrfunc:TID=%lu\n",pthread_self());

		//使线程保持一段时间,不至过快结束(对于分离性线程)
		//pthread_cond_timedwait();

		sleep(3);

		printf("Out thrfunc\n");
		pthread_exit(NULL);
}

int main()
{
		pthread_t tid;
		pthread_attr_t attr = {0};
		int ret;

		void* stackaddr_set = NULL;
		size_t stacksize_set;

		void* stackaddr_get = NULL;
		size_t stacksize_get;

		//初始化线程信息
		ret = pthread_attr_init(&attr);
		if(ret == 0){
				printf("thread attr init ok\n");
		}

		//线程信息设置分离性
		ret = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
		if(ret == 0){
				printf("thread set detach ok\n");
		}

		//设置线程栈空间和大小
		stacksize_set = 1024;
		stackaddr_set = (void*)malloc(stacksize_set);
		if(stackaddr_set != NULL){
				printf("malloc ok\n");
		}

		ret = pthread_attr_setstack(&attr, stackaddr_set, stacksize_set);
		if(ret == 0){
				printf("thread set stack ok:stackaddr=%p--stacksize=%d\n",stackaddr_set,stacksize_set);
		}
		else
		{
				printf("thread set stack error:%s\n",strerror(errno));
		}

		//创建线程
		ret = pthread_create(&tid,&attr,thrfunc,NULL);
		if(ret == 0){
				printf("thread create ok\n");
		}

		//获取线程属性:栈信息,此时线程还存活,没有释放资源
		ret =  pthread_attr_getstack(&attr, &stackaddr_get, &stacksize_get);
		if(ret == 0){
				printf("get stack info ok:stackaddr=%p--stacksize=%d\n",stackaddr_get,stacksize_get);
		}

		ret = pthread_attr_destroy(&attr);
		if(ret == 0){
				printf("thread destroy ok\n");
		}

		while(1){
				sleep(1);
		}

		pthread_exit(NULL);
}
