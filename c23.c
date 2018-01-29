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

//1 mutex互斥锁
/*
   pthread_mutex_t mutex;

   void *thrfunc(void *arg)
   {
   srand(time(NULL));
   int ret;
   while (1) {
   ret = pthread_mutex_lock(&mutex);
   if(ret == 0){
   printf("thread lock ok\n");
   }
   printf("hello ");
   sleep(rand() % 3);
   printf("world\n");
   ret = pthread_mutex_unlock(&mutex);
   if(ret == 0){
   printf("thread unlock ok\n");
   }
   sleep(rand() % 3);
   }
   return NULL;
   }

   int main(void)
   {
   int ret;
   pthread_t tid;
   srand(time(NULL)); 
   ret = pthread_mutex_init(&mutex,NULL);
   if(ret == 0){
   printf("mutex init ok\n");
   }
   pthread_create(&tid, NULL, thrfunc, NULL);
   while (1) {
   ret = pthread_mutex_lock(&mutex);
   if(ret == 0){
   printf("main lock ok\n");
   }
   printf("HELLO ");
   sleep(rand() % 3);
   printf("WORLD\n");
   ret = pthread_mutex_unlock(&mutex);
   if(ret == 0){
   printf("main unlock ok\n");
   }
   sleep(rand() % 3);
   }

   pthread_join(tid, NULL);
   ret = pthread_mutex_destroy(&mutex);
   return 0;
   }


//2 mutex,死锁1:重复加锁; 死锁2:互相阻塞死锁
pthread_mutex_t mutex1;
pthread_mutex_t mutex2;

void *thrfunc(void *arg)
{
srand(time(NULL));
int ret;
while (1) {
ret = pthread_mutex_lock(&mutex1); //lock: 先1后2
ret = pthread_mutex_lock(&mutex2);

printf("hello ");
sleep(rand() % 3);
printf("world\n");

ret = pthread_mutex_unlock(&mutex2); //unlock: 先2后1
ret = pthread_mutex_unlock(&mutex1);

sleep(rand() % 3);
}
return NULL;
}

int main(void)
{
		int ret;
		pthread_t tid;

		srand(time(NULL)); 
		ret = pthread_mutex_init(&mutex1,NULL);
		ret = pthread_mutex_init(&mutex2,NULL);
		pthread_create(&tid, NULL, thrfunc, NULL);

		while (1) {
				ret = pthread_mutex_lock(&mutex1); //lock: 先1后2
				ret = pthread_mutex_lock(&mutex2);
				//死锁2: 对于两个线程来说,线程2 lock:先2后1

				printf("HELLO ");
				sleep(rand() % 3);
				printf("WORLD\n");

				ret = pthread_mutex_unlock(&mutex2); //unlock: 先2后1
				ret = pthread_mutex_unlock(&mutex1);
				//死锁1: 对于一个线程自身来说:unlock:先1后2

				sleep(rand() % 3);
		}

		pthread_join(tid, NULL);
		ret = pthread_mutex_destroy(&mutex1);
		ret = pthread_mutex_destroy(&mutex2);

		return 0;
}


//3 读写锁应用
pthread_rwlock_t rwlock;
int value = 100;

void *thr_read(void *arg)
{
		while(1){
				pthread_rwlock_rdlock(&rwlock);
				printf("No.%02d rd_thread%lu: %d\n",(int)arg,pthread_self(),value);
				pthread_rwlock_unlock(&rwlock);
				usleep(1000000); //1s
		}
		pthread_exit(NULL);
}

void *thr_write(void *arg)
{
		while(1){
				pthread_rwlock_wrlock(&rwlock);
				value++;
				printf("No.%02d wr_thread%lu: %d\n",(int)arg,pthread_self(),value);
				pthread_rwlock_unlock(&rwlock);
				usleep(1000000); //1s
		}
		pthread_exit(NULL);
}

int main(void)
{
		int ret;
		pthread_t tid[10];

		ret = pthread_rwlock_init(&rwlock,NULL);
		if(ret == 0){
				printf("init rwlock ok\n");
		}

		int i;
		for(i = 0; i < 7; i++){
				//创建7个read线程
				pthread_create(&tid[i], NULL, thr_read, (void*)(i+1));
		}

		for(; i < 10; i++){
				//创建3个write线程
				pthread_create(&tid[i], NULL, thr_write, (void*)(i+1));
		}

		//阻塞回收子线程
		for(i = 0; i < 10; i++){
				pthread_join(tid[i], NULL);
		}

		//销毁读写锁
		pthread_rwlock_destroy(&rwlock);

		pthread_exit(NULL);
}
*/
/*
   No.06 rd_thread3034336064: 123
   No.10 wr_thread3000765248: 124
   No.07 rd_thread3025943360: 124
   */

//条件变量来实现生产者和消费者模型
pthread_cond_t cond;
pthread_mutex_t mutex;

typedef struct _PRODUCT
{
		int value;
		struct _PRODUCT* next;
}product;

struct _PRODUCT* head = NULL;
int beginnum = 1000;

void* productor(void* arg)
{
		while(1){
				pthread_mutex_lock(&mutex);//加锁

				//生产过程:加锁
				product* pnode = (product*)malloc(sizeof(struct _PRODUCT));
				pnode->value = beginnum++;
				pnode->next = head;
				head = pnode;

				pthread_mutex_unlock(&mutex); //解锁
				pthread_cond_signal(&cond); //生产完成后发送唤醒信号

				sleep(1);
		}

		pthread_exit(NULL);
}

void* customer(void* arg)
{
		while(1){
				pthread_mutex_lock(&mutex);

				while(head == NULL){
						pthread_cond_wait(&cond,&mutex);
				}
				product* pnode = head;
				head = head->next;
				printf("customer%d--value=%d--tid=%lu\n",(int)arg,pnode->value,pthread_self());
				free(pnode);

				pthread_mutex_unlock(&mutex);
				sleep(1);
		}
		pthread_exit(NULL);
}

int main()
{
		int ret;

		//初始化mutex, cond
		pthread_mutex_init(&mutex,NULL);
		pthread_cond_init(&cond,NULL);

		//创建线程,执行生产者1和消费者2任务
		pthread_t tid[3];
		pthread_create(&tid[0],NULL,productor,NULL);
		pthread_create(&tid[1],NULL,customer,(void*)1);
		pthread_create(&tid[2],NULL,customer,(void*)2);

		//回收线程
		int i;
		for(i = 0; i < 3; i++){
				pthread_join(tid[i],NULL);
		}

		//销毁:mutex, cond
		pthread_cond_destroy(&cond);
		pthread_mutex_destroy(&mutex);

		pthread_exit(NULL);
}
