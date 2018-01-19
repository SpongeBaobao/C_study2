#include <sys/types.h>
#include <sys/stat.h> 
#include <sys/wait.h>
#include <sys/mman.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//1
/*
   int main(){
   int i;
   pid_t pid;
   int pipefd[2];
   char buf[1024] = {0};

   pipe(pipefd); //建立管道

   for(i = 0; i<5; i++){
   pid = fork();
   if(pid == 0){
   int ret;
   printf("son%d:PID=%d\n",i+1,getpid());
   memset(buf,0,1024);
   sprintf(buf,"%d\n",getpid());
   close(pipefd[0]);
   ret = write(pipefd[1],buf,strlen(buf)); //将子进程id写入管道[123/n]
   if(ret > 0){
   printf("son%d write pipe ok:%s",i+1,buf);
   }
   close(pipefd[1]);

   break;
   }
   }

   if(i == 5){
//主进程
sleep(5);
close(pipefd[1]);
memset(buf,0,1024);
int ret = read(pipefd[0],buf,1024); //读取管道内容,获取5个子进程的pid
if(ret >  0){
printf("read pipe ok\n");
}
int pid1,pid2,pid3,pid4,pid5;
sscanf(buf,"%d\n%d\n%d\n%d\n%d\n",&pid1,&pid2,&pid3,&pid4,&pid5);
printf("parent: 5 for son pid:%d\t%d\t%d\t%d\t%d\n",pid1,pid2,pid3,pid4,pid5);

ret = kill(pid1,SIGKILL);
if(ret == 0){
printf("kill son%d ok\n",pid1);
}
sleep(1);

ret = kill(pid2,SIGKILL);
if(ret == 0){
printf("kill son%d ok\n",pid2);
}
sleep(1);

ret = kill(pid3,SIGKILL);
if(ret == 0){
printf("kill son%d ok\n",pid3);
}
sleep(1);

ret = kill(pid4,SIGKILL);
if(ret == 0){
printf("kill son%d ok\n",pid4);
}
sleep(1);

ret = kill(pid5,SIGKILL);
if(ret == 0){
printf("kill son%d ok\n",pid5);
}
sleep(1);


while(1){
		sleep(1);
}
}
else
{
		//子进程
		while(1){
				//printf("son%d living\n",getpid());
				sleep(1);
		}
}

return 0;
}
*/

//2 定时器
/*
   int main(){
   long long i = 0;
   unsigned int s = alarm(1); //设定一秒定时器

   while(1){
   printf("%lld\n",++i);
   }

   return 0;
   }
   */

//3 定时器2
/*
   unsigned int my_alarm(unsigned int t)
   {

   int ret;
   struct itimerval new_value = {{0,0}, {t,0}}; //设定时间{{周期}, {初始}};
   struct itimerval old_value = {0};

   ret = setitimer(ITIMER_REAL,&new_value,&old_value);
//if(ret == 0){
//		printf("set timer ok\n");
//}

return (unsigned int)old_value.it_value.tv_sec; //返回上次设置的时间:初始触发的秒数
}

int main()
{
int count = 0;
//alarm(1);
my_alarm(1);

while(1){
printf("%d\n",count++);
}

return 0;
}
*/

//3
//打印未决信号集
/*
   int main()
   {
// 获取当前未决信号集, 遍历检查常规信号的未决状态
sigset_t set;
int ret;
ret = sigemptyset(&set); //信号集置0
if(ret == 0){
printf("set empty ok\n");
}

ret = sigpending(&set);
if(ret == 0){
printf("sig pending ok\n");
}

int i;
printf("当前未决信号集: ");
for(i = 1; i <= 31; i++){
printf("%d",sigismember(&set,i));
}
printf("\n");

// 设置阻塞信号集: 常规信号皆阻塞
sigset_t set_block;
sigfillset(&set_block); //设置为全部阻塞
ret = sigprocmask(SIG_BLOCK,&set_block,NULL); //设置阻塞信号集
if(ret == 0){
printf("mask set ok\n");
}

// 触发常规信号,依次遍历当前未决信号集
for(i = 1; i <= 31; i++){
if(i== 9 || i==19) continue;

raise(i); //向自身发送信号,触发未决信号集
printf("发送%02d号信号; 当前未决信号集: ",i);
sleep(1);

ret = sigemptyset(&set);
ret = sigpending(&set); //获取未决信号集
int j;
for(j = 1; j <= 31; j++){
printf("%d",sigismember(&set,j));
}
printf("\n");
}

return 0;
}
*/

/*
   set empty ok
   sig pending ok
   当前未决信号集: 0000000000000000000000000000000
   mask set ok
   发送01号信号; 当前未决信号集: 1000000000000000000000000000000
   发送02号信号; 当前未决信号集: 1100000000000000000000000000000
   发送03号信号; 当前未决信号集: 1110000000000000000000000000000
   发送04号信号; 当前未决信号集: 1111000000000000000000000000000
   发送05号信号; 当前未决信号集: 1111100000000000000000000000000
   发送06号信号; 当前未决信号集: 1111110000000000000000000000000
   发送07号信号; 当前未决信号集: 1111111000000000000000000000000
   发送08号信号; 当前未决信号集: 1111111100000000000000000000000
   发送10号信号; 当前未决信号集: 1111111101000000000000000000000
   发送11号信号; 当前未决信号集: 1111111101100000000000000000000
   发送12号信号; 当前未决信号集: 1111111101110000000000000000000
   发送13号信号; 当前未决信号集: 1111111101111000000000000000000
   发送14号信号; 当前未决信号集: 1111111101111100000000000000000
   发送15号信号; 当前未决信号集: 1111111101111110000000000000000
   发送16号信号; 当前未决信号集: 1111111101111111000000000000000
   发送17号信号; 当前未决信号集: 1111111101111111100000000000000
   发送18号信号; 当前未决信号集: 1111111101111111110000000000000
   发送20号信号; 当前未决信号集: 1111111101111111100100000000000
   发送21号信号; 当前未决信号集: 1111111101111111100110000000000
   发送22号信号; 当前未决信号集: 1111111101111111100111000000000
   发送23号信号; 当前未决信号集: 1111111101111111100111100000000
   发送24号信号; 当前未决信号集: 1111111101111111100111110000000
   发送25号信号; 当前未决信号集: 1111111101111111100111111000000
   发送26号信号; 当前未决信号集: 1111111101111111100111111100000
   发送27号信号; 当前未决信号集: 1111111101111111100111111110000
   发送28号信号; 当前未决信号集: 1111111101111111100111111111000
   发送29号信号; 当前未决信号集: 1111111101111111100111111111100
   发送30号信号; 当前未决信号集: 1111111101111111100111111111110
   发送31号信号; 当前未决信号集: 1111111101111111100111111111111
   */

//4
//信号捕捉函数
void caughtfunc(int sig)
{
		printf("进入捕捉函数\n");
		printf("捕捉的信号:%d\n",sig);
		sleep(5);
		printf("离开捕捉函数\n");

		return;
}


int main()
{
		//设置捕捉函数执行期间的临时屏蔽信号集
		sigset_t maskset;
		int ret;
		ret = sigemptyset(&maskset);
		if(ret == 0){
				printf("set empty ok\n");
		}

		ret = sigaddset(&maskset,SIGINT); //捕获函数执行期间,临时屏蔽<Ctrl+C>
		if(ret == 0){
				printf("sig add ok\n");
		}

		//信号捕捉函数注册期间,临时屏蔽被捕捉函数
		sigset_t mask1, mask2;
		sigemptyset(&mask1);
		sigemptyset(&mask2);
		sigaddset(&mask1,SIGUSR1);
		ret = sigprocmask(SIG_BLOCK,&mask1,&mask2);
		if(ret == 0){
				printf("sigprocmask ok\n");
		}

		//注册信号捕捉函数
		struct sigaction act = {0};
		act.sa_flags = 0;
		act.sa_mask = maskset;
		act.sa_handler = caughtfunc;

		ret = sigaction(SIGUSR1,&act,NULL);
		if(ret == 0){
				printf("sigaction ok\n");
		}

		sigprocmask(SIG_SETMASK,&mask2,NULL); //注册完成后解除信号屏蔽 

		//测试信号捕捉函数
		int i = 1;
		while(i++ <= 3){
				raise(SIGUSR1);
				sleep(1);
				//3s间发送了3次信号
		}

		printf("test finished\n");

		return 0;
}
