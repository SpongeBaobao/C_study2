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

//strtok
/*
   int main(){
   char buf[1024] = "hel..lo.wor...ld";
   char *s = strtok(buf, ".");
   while (s != NULL) 
   { 
   printf("%s\n", s); 
   s = strtok(NULL, ".");
   }

   }
   */

// ip地址读取
/*
   union Space{
   unsigned char arr[4];
   unsigned int val;
   };

   unsigned int hton_ipv4(const char* ip, int len)
   {
   char buf[1024] = {0};
   memcpy(buf,ip,len);

   union Space a = {0};
   int i = 0;

   char* s = strtok(buf, ".");
//小端法向大端法转换
while(s != NULL){
//提取4次
a.arr[i] =(unsigned char)atoi(s);
i++;
s = strtok(NULL,".");
}
for(i = 0; i < 4; i++){
printf("%d\t",(int)a.arr[i]);
}

return a.val;
}

int main(){
char ip[64] = "192.168.137.255";
unsigned int ret;
ret = hton_ipv4(ip, strlen(ip));

printf("ret=%u\n",ret);

return 0;
}
*/

//验证子进程和父进程文件描述符是否共享
int main(){
		int fd = open("./f.txt",O_RDWR|O_CREAT|O_APPEND,0666);
		if(fd > 0){
				printf("open file ok\n");
		}

		pid_t pid = fork();
		if(pid == 0){
				printf("son:pid=%d--ppid=%d\n",getpid(),getppid());
				char buf[1024] = {0};
				//子进程读取信息
				int ret;
				//ret = read(fd,buf,1024);
				//if(ret > 0){
				//		printf("son read:%s",buf);
				//}
				
				//子进程写入信息
			//	memset(buf,0,1024);
				strcpy(buf,"son write in somethings\n");
				ret = write(fd,buf,strlen("son write in somethings\n"));
				if(ret > 0){
				printf("son write somethings\n");
				}
		}
		if(pid > 0){
				//子进程先读取
				sleep(2);
				printf("parent:pid=%d--spid=%d\n",getpid(),pid);
				char buf[1024] = {0};
				int ret;
				//文件光标置首
				//lseek(fd,0,SEEK_SET);
				strcpy(buf,"parent write in somethings\n");
				ret = write(fd,buf,strlen("parent write in somethings\n"));
				if(ret > 0){
				printf("parent write somethings\n");
				}

				//int ret = read(fd,buf,1024);
				//if(ret > 0){
				//		printf("parent read:%s",buf);
				//}

				wait(NULL);
		}

		return 0;
}
