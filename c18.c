#include <sys/types.h>
#include <sys/stat.h> 
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//3 无血缘进程间通信
//1
/*
   int main(int argc, char* argv[]){
   char buf[1024] = "hello!";
   int ret;
   int fd = open(argv[1],O_WRONLY);
   if(fd > 0){
   printf("p1 open ok\n");
   ret = write(fd,buf,strlen(buf));
   if(ret > 0){
   printf("p1 write ok\n");
   }
   }

   while(1){
   sleep(1);
   }

   return 0;
   }

//2
int main(int argc, char* argv[]){
char buf[1024] = {0};
int ret;
int fd = open(argv[1],O_RDONLY);
if(fd > 0){
printf("p2 open ok\n");
ret = read(fd,buf,sizeof(buf));
if(ret > 0){
printf("p2 read ok:[%s]\n",buf);
}
}

while(1){
sleep(1);
}

return 0;
}
*/

//mmap
#include <sys/mman.h>

//1 文件单方向向内存映射区同步数据
/*
   int main()
   {
//创建文件,作为内存映射的目标文件
int fd = open("./file",O_RDWR|O_CREAT,0777);
if(fd > 0){
printf("fife open ok\n");
}

//创建内存映射区
int memlen = 1024;
char* mem = mmap(NULL, memlen, PROT_READ,MAP_PRIVATE ,fd, 0); //内存映射只读,内存私有交互
if((int)mem != -1){
printf("mem creat ok\n");
}

int i = 0;

char buf_file[1024] = {0};
char buf_mem[1024] = {0};
int off = 0;
while(i++ <= 20){
//写文件
memset(buf_file,0,1024);
sprintf(buf_file,"hello%d",i);
write(fd,buf_file,strlen(buf_file));
printf("file write info:[%s]\n",buf_file);
sleep(1);

//读内存映射
memset(buf_mem,0,1024);
memcpy(buf_mem,mem + off,strlen(buf_file));
printf("mmap read info:[%s]\n",buf_mem);
off+=strlen(buf_file); //读取内存的起始点进行偏移
sleep(1);
}

//释放内存映射区
int ret = munmap(mem,memlen);
if(ret == 0){
printf("free mmap ok\n");
}

return 0;
}
*/

//2 父子进程间通信a
/*
   int main()
   {
   int fd = open("./file",O_RDWR|O_CREAT,0777);
   if(fd > 0){
   printf("open file ok\n");
   }

   char buf[1024] = "x";
   write(fd,buf,1);

   int memlen = 1024;
//char* mem = mmap(NULL,memlen,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0); //内存可读写,交互共享
char* mem = mmap(NULL,memlen,PROT_READ|PROT_WRITE,MAP_PRIVATE,fd,0); //内存可读写,交互私有
if((int)mem != -1){
printf("mmap creat ok\n");
}

pid_t pid = fork();

if(pid == 0){
//son
printf("son:PID=%d--PPID=%d\n",getpid(),getppid());
//子进程修改映射区内容
//1子进程向共享文件中写入,父子内存映射区自动同步
memset(buf,0,1024);
strcpy(buf,"123456789");
//int ret = write(fd,buf,strlen(buf));
//if(ret > 0){
//		printf("son write mmap ok,mem=[%p]\n",mem);
//}

//2子进程向内存映射区写入数据,不会同步到文件中
memcpy(mem,buf,strlen(buf)+1);
printf("son write mem ok,mem=[%p]\n",mem);

while(1){
sleep(1);
}

}

if(pid > 0){
//parent
sleep(2);
printf("parent:PID=%d--SPID=%d\n",getpid(),pid);

//读取内存映射区内容
memset(buf,0,1024);
memcpy(buf,mem,10);
printf("parent read info:[%s],mem=[%p]\n",buf,mem);

while(1){
sleep(1);
}
}

return 0;
}
*/
/*
MAP_PRIVATE: 父子进程数据不共享
open file ok
mmap creat ok
son:PID=3347--PPID=3346
son write mem ok,mem=[0xb7741000]
parent:PID=3346--SPID=3347
parent read info:[x],mem=[0xb7741000]
*/

//3 父子进程间通信b(两个子进程间相互通信,基于同一个共享文件)
/*
   int main()
   {
   int fd = open("./file",O_RDWR|O_CREAT,0777);
   if(fd > 0){
   printf("file open ok\n");
   }
   write(fd,"x",1); //保证文件大小不为0

   pid_t pid;
   int pipefd[2];
   int i;

   int ret = pipe(pipefd); //用于传输子进程的pid
   if(ret == 0){
   printf("pipe creat ok\n");
   }

   for(i = 0; i < 2; i++){
   pid = fork();
   if(pid == 0) break;
   }

   if(i == 0){
//son 1
close(pipefd[0]);
char str[32] = {0};
sprintf(str,"SPID1=%d\n",getpid());
write(pipefd[1],str,strlen(str));

int memlen1 = 1024;
char* mem1 = mmap(NULL,memlen1,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
if((int)mem1 != -1){
printf("mmap1 creat ok,ad=[%p]\n",mem1);
}

char buf1[1024] = "helloworld";
memcpy(mem1,buf1,5); //第一次写入"hello"
printf("son1: write into mem1:hello\n");
sleep(1);
memcpy(mem1+5,buf1+5,5); //第二次写入"world"
printf("son1: write into mem1:world\n");

while(1){
sleep(1);
}

}

if(i == 1){
//son2
close(pipefd[0]);
char str[32] = {0};
sprintf(str,"SPID2=%d\n",getpid());
write(pipefd[1],str,strlen(str));

int memlen2 = 1024;
char* mem2 = mmap(NULL,memlen2,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
if((int)mem2 != -1){
printf("mmap2 creat ok,ad=[%p]\n",mem2);
}

sleep(5);//等待son1写完
char buf2[1024] = {0};
memcpy(buf2,mem2,5); //第一次读取
printf("son2:read mem2:[%s]\n",buf2);
sleep(1);
memset(buf2,0,1024);
memcpy(buf2,mem2+5,5); //第二次读取
printf("son2:read mem2:[%s]\n",buf2);

while(1){
		sleep(1);
}

}

if(i == 2){
		//parent
		sleep(8);
		close(pipefd[1]);
		char buf[1024] = {0};
		int ret = read(pipefd[0],buf,sizeof(buf));
		if(ret > 0){
				printf("parent's son info:\n%s",buf);
		}

		while(1){
				sleep(1);
		}
}

return 0;
}
*/
/*
   file open ok
   pipe creat ok
   mmap1 creat ok,ad=[0xb7701000]
   mmap2 creat ok,ad=[0xb7701000]
   son1: write into mem1:hello
   son1: write into mem1:world
   son2:read mem2:[hello]
   son2:read mem2:[world]
   parent's son info:
   SPID1=3447
   SPID2=3448
   */

//4 无血缘关系进程间通信
int main(int argc, char* argv[]){
		int fd = open(argv[1],O_RDWR|O_CREAT,0777);
		if(fd > 0){
				printf("file open ok\n");
		}
		int ret;
		int memlen = 32;
		ret = truncate(argv[1],memlen); //拓展文件大小
		if(ret == 0){
				printf("truncate file ok\n");
		}

		char* mem = mmap(NULL,memlen,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
		if(mem != MAP_FAILED){
				printf("mmap create ok\n");
		}
		close(fd); //创建mmap后关闭文件

		char buf[1024] = {0};
		int num = 0;
		while(num++ <= 100){
				sprintf(buf,"hello%03d\n",num);
				memcpy(mem,buf,strlen(buf)+1);
				memset(buf,0,1024);
				sleep(1);
		}

		ret = munmap(mem,memlen);
		if(ret == 0){
				printf("free mmap ok\n");
		}

		return 0;
}

//2
int main(int argc, char* argv[]){
		int fd = open(argv[1],O_RDWR|O_CREAT,0777);
		if(fd > 0){
				printf("2:file open ok\n");
		}
		int ret;
		int memlen = 32;
		ret = truncate(argv[1],memlen); //拓展文件大小
		if(ret == 0){
				printf("2:truncate file ok\n");
		}

		char* mem = mmap(NULL,memlen,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
		if(mem != MAP_FAILED){
				printf("2:mmap create ok\n");
		}
		close(fd); //创建mmap后关闭文件

		char buf[1024] = {0};
		int num = 0;
		while(num++ <= 100){
				memset(buf,0,1024);
				memcpy(buf,mem,10); //读取mmap中的数据
				printf("read:%s",buf);
				sleep(1);
		}

		ret = munmap(mem,memlen);
		if(ret == 0){
				printf("free mmap ok\n");
		}

		return 0;
}
