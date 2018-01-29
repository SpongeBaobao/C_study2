#include <sys/types.h>
#include <sys/stat.h> 
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
#include <errno.h>

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//1 信号量实现生产者和消费者模型
/*
#define N 10
int queue[N] = {0};  // 模拟环形队列
sem_t sem_need;
sem_t sem_give;
int p_index = 0; //累计生产数量,当前生产标号(p_index)%N
int c_index = 0; //累计消费数量,当前消费标号(c_index)%N

void* productor(void* arg)
{
//生产者逻辑:将空位置置1
while(1){
sem_wait(&sem_need); //need--
if(queue[p_index % N] == 0){
queue[p_index % N] = 1;
p_index++;
}
printf("productor%d--count=%d--index=%d\n",(int)arg,p_index,(p_index-1)%N);
sem_post(&sem_give); //give++

sleep(1);
}

return NULL;
}

void* customer(void* arg)
{
//消费者逻辑:将满位置置0
while(1){
sem_wait(&sem_give); //give--
if(queue[c_index % N] == 1){
queue[c_index % N] = 0;
c_index++;
}
printf("customer%d--count=%d--index=%d\n",(int)arg,c_index,(c_index-1)%N);
sem_post(&sem_need); //need++

sleep(1);
}

return NULL;
}

int main()
{
//信号量初始化
//生产者初始情况:需求need=10,供给give=0
sem_init(&sem_need, 0, N); 
sem_init(&sem_give, 0, 0);

int i;
int pcount = 2; //生产者数量
int ccount = 2; //消费者数量
pthread_t ptid[pcount];
pthread_t ctid[ccount];

//创建生产者线程1
for(i = 0; i < pcount; i++){
pthread_create(&ptid[i],NULL,productor,(void*)(i+1));
}
//创建消费者线程2
for(i = 0; i < ccount; i++){
pthread_create(&ctid[i],NULL,customer,(void*)(i+1));
}

//主线程逻辑:
//报告累计生产数量和消费数量
while(1){
printf("main: 生产总量=%d--消费总量=%d--生产盈余=%d\n",p_index,c_index,p_index-c_index);
sleep(1);
}

//回收线程
for(i = 0; i < pcount; i++){
		pthread_join(ptid[i],NULL);
}

for(i = 0; i < ccount; i++){
		pthread_join(ctid[i],NULL);
}

//销毁信号量
sem_destroy(&sem_need);
sem_destroy(&sem_give);

pthread_exit(NULL);
}
*/

/*
   productor2--count=130--index=9
   customer2--count=129--index=8
   customer1--count=130--index=9
   productor1--count=131--index=0
   customer1--count=131--index=0
   productor2--count=132--index=1
   customer2--count=132--index=1
   main: 生产总量=132--消费总量=130--生产盈余=2
   */


//2 文件锁的应用
#define FILEPATH "/home/zc/EX2/filelock_test.txt"
int main()
{
		int fd = open(FILEPATH,O_RDWR|O_CREAT|O_APPEND,0666);
		if(fd > 0){
		printf("open file ok\n");
		}

		struct flock fk = {0};
		fk.l_type = F_WRLCK; //锁的类型:write lock
		fk.l_whence = SEEK_SET; //锁作用起始位置
		fk.l_start = 0; //偏移量
		fk.l_len  = 0;  //锁作用长度

		int ret = fcntl(fd,F_SETLK,&fk); //设置锁
		if(ret == 0){
				printf("file set lock ok\n");
		}

		sleep(10); //锁作用生效时间

		ret = fcntl(fd,F_UNLCK);
		if(ret == 0){
				printf("file unlock ok\n");
		}
		close(fd);

		return 0;
}
