#include <sys/types.h>
#include <sys/stat.h> 
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>

//1 pipe
/*
   int main(){
   int pipefd[2];
   int ret = pipe(pipefd);

   if(ret == 0){
   printf("pipe ok\n");
   }
   else{
   return -1;
   }

   pid_t pid = fork();

   if(pid == 0){
   printf("son:PID=%d--PPID=%d\n",getpid(),getppid());
   close(pipefd[1]); //close write

   char buf[1024] = {0};
   ret = read(pipefd[0],buf,sizeof(buf));
   if(ret > 0){
   printf("son: read ok: %s\n",buf);
   sleep(1);
   }
   }

   if(pid >0){
   printf("parent:PID=%d--SPID=%d\n",getpid(),pid);
   close(pipefd[0]); //close read

   char buf[1024] = "study up!!!";
   ret = write(pipefd[1],buf,(unsigned int)strlen(buf)); //write info
   if(ret > 0){
   printf("parent sent ok\n");

   wait(0);

   printf("parent: wait ok\n");
   while(1){
   sleep(1);
   }
   }
   }

   return 0;
   }
   */
/*
   pipe ok
parent:PID=2925--SPID=2926
parent sent ok
son:PID=2926--PPID=2925
son: read ok: study up!!!
parent: wait ok
*/

//2: 子进程产生SIGPIPE
/*
   int main(){
   int pipefd[2];
   int ret = pipe(pipefd);

   pid_t pid = fork();

   if(pid == 0){
   sleep(1); //等待父进程关闭读写端
   printf("son:PID=%d--PPID=%d\n",getpid(),getppid());
   close(pipefd[0]);

   char buf[1024] = "son msg";
   ret = write(pipefd[1],buf,sizeof(buf));
   if(ret > 0){
   printf("son: sent ok: %s\n",buf);
   sleep(1);
   }
   }

   if(pid >0){
   printf("parent:PID=%d--SPID=%d\n",getpid(),pid);
   close(pipefd[0]);
   close(pipefd[1]);

   int status;
   wait(&status);
   printf("parent: wait ok\n");
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

   while(1){
   sleep(1);
   }
   }
   return 0;
   }
   */

//3 写端关闭
/*
   int main(){
   int pipefd[2];
   int ret = pipe(pipefd);

   pid_t pid = fork();

   if(pid == 0){
   sleep(1); //等待父进程关闭读写端
   printf("son:PID=%d--PPID=%d\n",getpid(),getppid());
   close(pipefd[1]);

   char buf[1024] = {0};
   ret = read(pipefd[0],buf,sizeof(buf));
   printf("read ret = %d\n",ret);
   sleep(1);
   }

   if(pid >0){
   printf("parent:PID=%d--SPID=%d\n",getpid(),pid);
   close(pipefd[0]);
//close(pipefd[1]);

int status;
wait(&status);
printf("parent: wait ok\n");
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

while(1){
sleep(1);
}
}
return 0;
}
*/

//4 读端不读,写端狂写
/*
   int main(){
   int pipefd[2];
   int ret = pipe(pipefd);

   pid_t pid = fork();

   if(pid == 0){
   sleep(1);
   printf("son:PID=%d--PPID=%d\n",getpid(),getppid());
   close(pipefd[0]);//关闭读端

   char buf[1024] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
   long long num = 0;
   while((ret = write(pipefd[1],buf,32)) > 0 ){
   num+=ret;
   printf("num=%lld\n",num);
   }

   printf("write fin:num=%lld--ret=%d\n",num,ret);

   sleep(1);
   }

   if(pid >0){
   printf("parent:PID=%d--SPID=%d\n",getpid(),pid);
   close(pipefd[1]);

   int status;
   wait(&status);
   printf("parent: wait ok\n");
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
   int pipefd[2];
   int ret = pipe(pipefd);

   int i;
   for(i = 0; i < 2; i++){
   pid_t pid = fork();
   if(pid == 0) break;
   }

   if(i == 1){
//第二个子进程
printf("son2:PID=%d--PPID=%d\n",getpid(),getppid());
close(pipefd[1]); //关闭write,只读

dup2(pipefd[0],STDIN_FILENO); //pipefd[0]==stdin
execlp("grep","grep","bash",NULL); //grep从pipefd[0]读取数据
}

if(i == 0){
//第一个子进程
//开始创建son1,父进程未关闭pipefd,son2未创建: 不会造成SIGPIPE情况
printf("son1:PID=%d--PPID=%d\n",getpid(),getppid());
close(pipefd[0]);//关闭读端,只写

dup2(pipefd[1],STDOUT_FILENO); //pipefd[1]==stdout
execlp("ps","ps","aux",NULL); //ps将结果写入pipefd[1]
}

if(i == 2){
//父进程
printf("parent:PID=%d\n",getpid());
close(pipefd[0]);
close(pipefd[1]);

while(1){
ret = waitpid(-1,0,WNOHANG);
if(ret > 0) { printf("回收一个子进程:pid=%d\n",ret); }
if(ret == -1) { printf("没有可回收的子进程\n"); break; }
}

}
return 0;
}
*/

//查看系统设置
/*
   int main(){
   long pipe_num;
   long filename_num;

   pipe_num = pathconf("/etc/security/limits.conf", _PC_PIPE_BUF);
   if(pipe_num != -1){
   printf("pipenum=%ld\n",pipe_num);
   }

   filename_num = pathconf("/etc/security/limits.conf", _PC_NAME_MAX);
   if(filename_num != -1){
   printf("filenamenum=%ld\n",filename_num);
   }

   return 0;
   }
   */

/*
   pipenum=4096
   filenamenum=255
   */

//FIFO
//一个写端,两个读端
int main(int argc, char* argv[])
{
		//argv[1] == "./FIFO" == 利用mkfifo创建的管道文件
		int ret = mkfifo("./myfifo",0777);
		if(ret == -1){
				printf("mkfifo error\n");
				return -1;
		}
		else{
				printf("mkfifo ok\n");
		}

		int fd1 = open("myfifo",O_RDWR);
		int fd2 = open("myfifo",O_RDWR);
		int fd3 = open("myfifo",O_RDWR);

		if(!((fd1>0)&&(fd2>0)&&(fd3>0))){
				printf("open err\n");
				return -2;
		}
		//fd1 wrute, fd2 & fd3 read
		char buf[1024] = "helloworld";
		ret = write(fd1,buf,strlen(buf));
		if(ret > 0){
				printf("fd1 write ok,ret=%d\n",ret);
		}
		else{
				return -3;
		}

		memset(buf,0,1024);
		ret = read(fd2,buf,5);
		if(ret > 0){
				printf("fd2 read:%s\n",buf);
		}
		else{
				return -3;
		}

		memset(buf,0,1024);
		ret = read(fd3,buf,5);
		if(ret > 0){
				printf("fd3 read:%s\n",buf);
		}
		else{
				return -3;
		}

		return 0;
}


//父进程向子进程传递信息
int main(int argc, char* argv[])
{
		//FIFO文件==argv[1]
		if(argc != 2){
				printf("参数无效\n");
				return -1;
		}

		int fd;
		int ret;
		pid_t pid = fork();

		if(pid == 0){
				//子进程
				printf("son:PID=%d--PPID=%d\n",getpid(),getppid());
				fd = open(argv[1],O_RDONLY);
				if(fd > 0){
						printf("son open fifo ok\n");
						char buf[1024] = {0};
						ret = read(fd,buf,sizeof(buf));
						if(ret > 0){
								printf("son read ok:[%s]\n",buf);
						}
						sleep(2);
						printf("son will be died\n");
				}
		}

		if(pid > 0){
				//父进程
				printf("parent:PID=%d--SPID=%d\n",getpid(),pid);
				fd = open(argv[1],O_WRONLY);
				if(fd > 0){
						printf("parent open fifo ok\n");
						char buf[1024] = "study up";
						ret = write(fd,buf,strlen(buf));
						if(ret > 0){
								printf("parent write ok\n");
						}		
				}

				ret = wait(0);
				printf("parent wait ok\n");

				ret = unlink(argv[1]);
				if(ret == 0){
						printf("parent remove fifo file ok\n");
				}

				while(1){
						sleep(1);
				}
		}
		return 0;
}
/*
parent:PID=2955--SPID=2956
son:PID=2956--PPID=2955
son open fifo ok
parent open fifo ok
parent write ok
son read ok:[study up]
son will be died
parent wait ok
parent remove fifo file ok
*/

