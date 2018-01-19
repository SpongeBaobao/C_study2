#include <sys/types.h>
#include <sys/stat.h> 
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//1 dup函数
/*
   int main()
   {
   int ret;

//文件描述符复制函数
printf("STDIN_FILENO:%d\n",STDIN_FILENO);
printf("STDOUT_FILENO:%d\n",STDOUT_FILENO);
printf("STDERR_FILENO:%d\n",STDERR_FILENO);

int fd1 = dup(STDOUT_FILENO);
if(fd1 != -1){
printf("dup fd1:%d\n",fd1);
write(fd1,"fd1-->stdout\n",strlen("fd1-->stdout\n"));
write(STDOUT_FILENO,"stdout\n",strlen("stdout\n"));
}

int fd2 = open("./test.txt",O_RDWR|O_CREAT|O_APPEND, 0777);
if(fd2>0){
printf("fd2-->testfile open ok\n");
printf("fd2=%d\n",fd2);
}

ret = dup2(fd2,1);
if(ret != -1){
//test:通过printf输入fd2文件中
printf("want to printf stdout\n");
}


char buf[1024] = {0};
ret = read(fd2,buf,1024);
if(ret > 0){
close(1);
close(fd2);
}

ret = write(fd1,buf,strlen(buf));

return 0;
}
*/

//2
/*
   int main(){

   pid_t pid = fork();
   int n;
   char buf[20] = {0};
   if(pid == 0){
   printf("son pro:pid=%d--ppid=%d--uid=%d--gid=%d\n",getpid(),getppid(),getuid(),getgid());
   strcpy(buf,"son 3s\n");

   n = 3;
   }
   else{
   printf("parent pro:pid=%d--sonpid=%d--uid=%d--gid=%d\n",getpid(),pid,getuid(),getgid());
   strcpy(buf,"parent 6s\n");
   n = 6;
   }

   for(;n>0; n--){
   printf("%s",buf);
   sleep(1);
   }

   return 0;
   }
   */

//3 产生10个子进程
/*
   int main(){
   int i;
   pid_t pid;

   for(i = 0; i < 10; i++){
   pid = fork();

   if(pid == 0){
   printf("son pro:pid = %d\n",getpid());
   sleep(1);
   break;
   }
   else{
   printf("parent:%d--%d\n",i+1,pid);
   sleep(1);
   }
   }

   return 0;
   }
   */

//4 验证全局变量是否共享

int x = 100;

int main(){
		pid_t pid = fork();
		if(pid == 0){
				while(1){
						printf("son:pid=%d--ppid=%d--x=%d\n",getpid(),getppid(),x);
						sleep(2);
				}
		}
		else{
				printf("parent:pid=%d--sonpid=%d--x=%d\n",getpid(),pid,x);
				sleep(6);
				x=200;
				printf("parent:change\n");
				while(1){
						sleep(1);
						printf("parent:x=%d\n",x);
				}
		}

		return 0;
}
