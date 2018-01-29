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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//1  ttyname
int main1()
{
		int fd = open("./file.txt",O_RDWR);
		if(fd > 0){
				printf("open ok\n");
		}

		printf("fd ttyname:[%s]\n",ttyname(STDOUT_FILENO));

		return 0;
}
//普通文件: fd ttyname:[(null)]
//std: fd ttyname:[/dev/pts/1]

//2 创建会话
int main2()
{
		pid_t pid = fork();

		if(pid == 0){
				//son
				printf("son:PID=%d--PPID=%d--GPID=%d\n",getpid(),getppid(),getpgrp());
				sleep(1);

				//create session
				pid = setsid();
				if(pid != -1){
						printf("create session ok\n");
						printf("son:PID=%d--PPID=%d--GPID=%d--SID=%d\n",getpid(),getppid(),getpgrp(),getsid(0));

						while(1){
								sleep(1); //脱离控制控制终端
						}
				}

		}

		if(pid > 0){
				//parent
				printf("parent:PID=%d--SPID=%d--GPID=%d\n",getpid(),pid,getpgrp());
				sleep(1); //父进程结束
		}

		return 0;
}
/*
parent:PID=3192--SPID=3193--GPID=3192
son:PID=3193--PPID=3192--GPID=3192
create session ok
son:PID=3193--PPID=1--GPID=3193--SID=3193
*/

//3 创建守护进程tim.out
static int count = 0; //全局变量,计数器
void caughtfunc(int sig);

int main3()
{
		//fork
		pid_t pid = fork();
		if(pid > 0){
				//parent
				printf("parent:PID=%d--SPID=%d--GPID=%d\n",getpid(),pid,getpgrp());
		}

		if(pid == 0){
				//son
				int ret;
				printf("son:PID=%d--PPID=%d--GPID=%d\n",getpid(),getppid(),getpgrp());

				//create session
				ret = setsid();
				if(ret != -1){
						printf("create session ok\n");
						printf("son:PID=%d--PPID=%d--GPID=%d--SID=%d\n",getpid(),getppid(),getpgrp(),getsid(0));
				}
				//chdir
				ret = chdir("/home/zc/EX2/mydaemon");
				if(ret == 0){
						printf("chdir ok\n");
				}

				//umask
				ret = umask(0002);
				if(ret == 0002){
						printf("umask ok\n");
				}

				//close fd

				//创建日志文件
				int fd = open("/home/zc/EX2/mydaemon/tim.log",O_RDWR|O_CREAT|O_APPEND,0666);
				if(fd > 0){
				printf("create logfile ok\n");
				}
				close(fd);

				//main work : 记录存活的时间
				//注册捕捉函数
				struct sigaction act = {0};
				act.sa_handler = caughtfunc;
				sigemptyset(&act.sa_mask);
				act.sa_flags = 0;

				ret = sigaction(SIGALRM, &act, NULL);
				if(ret == 0){
						printf("caught func ok\n");
				}

				//设置循环时钟:心跳检测
				struct itimerval value = {{60,0},{10,0}}; //循环时间1min
				ret = setitimer(ITIMER_REAL, &value,NULL);
				if(ret == 0){
						printf("set alarm ok\n");
				}

				while(1){
				sleep(1);
				}
		}

		return 0;
}

void caughtfunc(int sig)
{
		int fd = open("/home/zc/EX2/mydaemon/tim.log",O_WRONLY|O_APPEND);
		char buf[1024] = {0};
		if(count == 0){
				write(fd,"tim daemon start\n",17);
				count++;
		}
		else
		{
				sprintf(buf,"%03d\n",count);
				write(fd,buf,strlen(buf));
				count++;
		}
		close(fd);

		return;
}
