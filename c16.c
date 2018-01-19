#include <sys/types.h>
#include <sys/stat.h> 
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <sys/wait.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//1 fcntl
/*	
	int main(){
//1
//int fd1 = dup(STDOUT_FILENO);
//int fd2 = fcntl(1, F_DUPFD, 0);

//write(fd1,"dup func\n",10);
//write(fd2,"fcntl func\n",12);

//2 将stdin设置为非阻塞
int flag = fcntl(STDIN_FILENO,F_GETFL);
printf("stdin getfl=%d\n",flag);
fcntl(STDIN_FILENO,F_SETFL,flag|O_NONBLOCK);

//test
int a;
printf("输入一个整数\n");
scanf("%d",&a);

return 0;
}
*/

//2 内存对齐
/*
#pragma pack(8)

typedef struct _STUDENT{
int a;
char b;
double c;
float d;
}Student;

typedef struct _STUDENT2{
char a;
Student b;
double c;
}Student2;

void main(){
printf("sizeof(float):%u\n",sizeof(float));
printf("sizeof(double):%u\n",sizeof(double));

printf("sizeof Student:%d\n",sizeof(Student));
printf("sizeof Student2:%d\n", sizeof(Student2));
}
*/

//exec函数
/*
   int main(){
   int fd1 = open("./test1",O_RDWR|O_CREAT|O_APPEND, 0777);
   int fd2 = open("./test2",O_RDWR|O_CREAT|O_APPEND, 0777);

   pid_t  pid;
   int i;
   for(i = 0; i < 2; i++){
   pid = fork();
   if(pid == 0)
   break;
   }

//第一个子进程:
if(i == 0){
printf("i=%d--pid=%d--PID=%d--PPID=%d\n",i,pid,getpid(),getppid());
dup2(fd1,STDOUT_FILENO); //stdout-->fd1
printf("son1:execlp(ps-aux)\n");
int ret = execlp("ps","ps","aux",NULL);
if(ret == -1){
printf("pro1 execlp err\n");
}
}

//第二个子进程:
if(i == 1){
printf("i=%d--pid=%d--PID=%d--PPID=%d\n",i,pid,getpid(),getppid());
dup2(fd2,STDOUT_FILENO); //stdout-->fd2
printf("son2:execl(ps-ajx)\n");
int ret = execl("/bin/ps","ps","ajx",NULL);
if(ret == -1){
printf("pro2 execl err\n");
}

}

//主进程:
if(i == 2){
printf("i=%d--PID=%d\n",i,getpid());
while(1){
sleep(1);
}
}

return 0;
}
*/

//
/*
   int main(){
   int fd1 = open("./test1",O_RDWR|O_CREAT|O_APPEND, 0777);

   pid_t  pid;
   int i;
   for(i = 0; i < 2; i++){
   pid = fork();
   if(pid == 0)
   break;
   }

//第一个子进程:
if(i == 0){
printf("i=%d--pid=%d--PID=%d--PPID=%d\n",i,pid,getpid(),getppid());
dup2(fd1,STDOUT_FILENO); //stdout-->fd1
}

//第二个子进程:
if(i == 1){
sleep(3);
printf("i=%d--pid=%d--PID=%d--PPID=%d\n",i,pid,getpid(),getppid());
}

//主进程:
if(i == 2){
printf("i=%d--PID=%d\n",i,getpid());
while(1){
sleep(1);
}
}

return 0;
}
*/

//进程回收wait()
/*
   int main(){
   pid_t pid = fork();
   if(pid == 0){
   printf("son--PID%d--PPID%d\n",getpid(),getppid());
   int i = 0;
   while(1){
//子进程sleep3秒后结束
if(i == 3) break;
printf("son--%d\n",i+1);
i++;
sleep(1);
}
}

if(pid != 0){
int status;
printf("parent--PID%d--SPID%d\n",getpid(),pid);
pid = wait(&status);
if (WIFEXITED(status))
{
//WEXITSTATUS(status);
printf("进程正常退出:[%d].\n", WEXITSTATUS(status));
}

if (WIFSIGNALED(status))
{
//WTERMSIG(status);
printf("进程异常终止:[%d].\n", WTERMSIG(status));
}

if (WIFSTOPPED(status))
{
//WSTOPSIG(status);
printf("进程暂停:[%d].\n", WSTOPSIG(status));
}

if (WIFCONTINUED(status))
{
printf("进程暂停后继续运行.\n");
}

while(1){
sleep(1);
}
}

return 0;
}
*/

//进程回收waitpid()
/*
   int main(){
   pid_t pid = fork();
   if(pid == 0){
   printf("son--PID%d--PPID%d\n",getpid(),getppid());
   int i = 0;
   while(1){
//子进程sleep3秒后结束
if(i == 3) break;
printf("son--%d\n",i+1);
i++;
sleep(1);
}
}

if(pid != 0){
int status;
printf("parent--PID%d--SPID%d\n",getpid(),pid);
//阻塞回收所有进程,等效于wait
//pid = waitpid(-1,&status,0);

//非阻塞回收所有进程
while(1){
pid = waitpid(-1,&status,WNOHANG);

if(pid == 0) { printf("还有运行中的子进程\n"); }
if(pid > 0) { printf("回收一个子进程:pid=%d\n",pid); }
if(pid == -1) { printf("没有可回收的子进程\n"); break; }

sleep(1);
}

if (WIFEXITED(status))
{
//WEXITSTATUS(status);
printf("进程正常退出:[%d].\n", WEXITSTATUS(status));
}

if (WIFSIGNALED(status))
{
//WTERMSIG(status);
printf("进程异常终止:[%d].\n", WTERMSIG(status));
}

if (WIFSTOPPED(status))
{
//WSTOPSIG(status);
printf("进程暂停:[%d].\n", WSTOPSIG(status));
}

if (WIFCONTINUED(status))
{
printf("进程暂停后继续运行.\n");
}

while(1){
sleep(1);
}
}

return 0;
}
*/

//
int main(){
		int fd = open("./test",O_RDWR|O_CREAT|O_APPEND, 0777);

		pid_t  pid;
		int i;
		for(i = 0; i < 3; i++){
				pid = fork();
				if(pid == 0)
						break;
		}

		//第一个子进程
		if(i == 0){
				printf("i=%d--pid=%d--PID=%d--PPID=%d\n",i,pid,getpid(),getppid());
				dup2(fd,STDOUT_FILENO);
				execlp("ps","ps","ajx",NULL);
				sleep(i+1);

		}

		//第二个子进程
		if(i == 1){
				printf("i=%d--pid=%d--PID=%d--PPID=%d\n",i,pid,getpid(),getppid());
				execl("./myapp.out","myapp.out","son2",NULL);
				sleep(i+1);

		}

		//第三个子进程
		if(i == 2){
				printf("i=%d--pid=%d--PID=%d--PPID=%d\n",i,pid,getpid(),getppid());
				char* p = "helloworld\n";
				*p = 'H';
				sleep(i+1);
		}

		//主进程
		if(i == 3){
				int status;
				printf("parent--PID%d--SPID%d\n",getpid(),pid);

				while(1){
						pid = waitpid(-1,&status,WNOHANG);
						if(pid > 0) {		
								printf("回收一个子进程:pid=%d,结束原因:",pid);
								if (WIFEXITED(status))
								{
										printf("进程正常退出:[%d].\n", WEXITSTATUS(status));
								}

								if (WIFSIGNALED(status))
								{
										printf("进程异常终止:[%d].\n", WTERMSIG(status));
								}

								if (WIFSTOPPED(status))
								{
										printf("进程暂停:[%d].\n", WSTOPSIG(status));
								}

								if (WIFCONTINUED(status))
								{
										printf("进程暂停后继续运行.\n");
								}

						}

						if(pid == -1) {
								printf("没有可回收的子进程\n");
								break; 
						}
				}

				while(1){
						sleep(1);
				}
		}

		return 0;
}
