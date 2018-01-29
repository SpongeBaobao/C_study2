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

//1 bigfile r&w
/*
   void calsize(long long size);

   int main(int argc, char* argv[])
   {
   if(argc != 3){
   printf("arg err");
   return -1;
   }

   int fd_r = open(argv[1],O_RDONLY);
   int fd_w = open(argv[2],O_WRONLY|O_CREAT|O_APPEND,0666);
   if(fd_r == -1 || fd_w == -1){
   printf("open err\n");
   return -2;
   }

   int len = 1024;
   int ret;
   char buf[1024]={0};
   long long size = 0;
   while((ret = read(fd_r,buf,len)) > 0 ){
   ret = write(fd_w,buf,len);
   size += ret;
   memset(buf,0,len);
   }

   printf("file copy ok\n");
   calsize(size);

   close(fd_r);
   close(fd_w);

   return 0;
   }

   void calsize(long long size)
   {
   long long sb = size;
   long long sk = size/1024;
   long long sm = sk/1024;

   printf("size:%lldbyte--%lldK--%lldM\n",sb,sk,sm);

   return;
   }
   */

//2 lstat & stat
/*
   int main(int argc, char* argv[])
   {
//argv[1]为软链接文件
struct stat st = {0};
struct stat st_sl = {0};

int ret;
//获取文件信息
ret = stat(argv[1],&st);
if(ret == 0){
printf("stat ok\n");
}

ret = lstat(argv[1], &st_sl);
if(ret == 0){
printf("lstat ok\n");
}

printf("stat: size=%lu -- ino=%lu\n",st.st_size,st.st_ino);
printf("lstat: size=%lu -- ino=%lu\n",st_sl.st_size,st_sl.st_ino);

return 0;
}
*/
/*
stat: size=683205881 -- ino=156955
lstat: size=26 -- ino=169282
*/

//3 fcntl
/*
   int main()
   {
   char ch;
   printf("1: input a character\n");
   scanf("%c",&ch);
   printf("1: get a character: %c\n",ch);

   int fd = fcntl(STDIN_FILENO, F_DUPFD, 0); //复制文件描述符
   if(fd != -1){
   printf("复制文件描述符ok\n");
   }
//将stdin设置为非阻塞状态
int flag = fcntl(fd, F_GETFL);
if(flag != -1){
printf("目标文件状态获取ok\n");
}
flag = flag | O_NONBLOCK;
int ret = fcntl(fd, F_SETFL, flag);
if(ret != -1){
printf("文件状态设置为非阻塞ok\n");
}

sleep(2);

printf("2: input a character\n");
scanf("%c",&ch);
printf("2: get a character: %c\n",ch);

return 0;
}
*/

// 获取1000-2000进程编号的程序
int check_line(char* buf, int len);

int main(int argc, char* argv[])
{
		int ret;
		pid_t pid = fork();

		if(pid == 0){
				//son
				printf("son:pid=%d--ppid=%d\n",getpid(),getppid());
				int fd = open(argv[1],O_RDWR|O_CREAT|O_APPEND,0666);
				if(fd > 0){
						printf("son: open and create file ok\n");
				}

				//重定向文件描述符:
				dup2(fd,STDOUT_FILENO);

				//执行execlp,进行ps -aux 进程
				execlp("ps","ps","aux",NULL);

		}
		else
		{
				//parent
				sleep(1);
				printf("parent:pid=%d--spid=%d\n",getpid(),pid);

				//回收子进程
				ret = wait(NULL);
				if(ret != -1){
						printf("wait ok\n");
				}

				//读取文件内容
				FILE* fp = fopen(argv[1],"r");
				if(fp != NULL){
						printf("parent: fopen file ok\n");
				}

				char buf[1024] = {0}; //用于存储文件一行字符串信息
				int row_count = 0; //文件行数
				int id; //每行的进程id
				int id_count = 0; //有效的进程id数量

				//文件行读取
				while(fgets(buf,sizeof(buf),fp) != NULL)
				{
						row_count++;
						if(row_count == 1){
								memset(buf,0,sizeof(buf));
								continue; //跳过第一行(标题行)	
						}

						//id =  check_line(buf, strlen(buf)); //读取文本行,提取进程id
						id =  check_line2(buf, strlen(buf)); //读取文本行,提取进程id
						if(id >= 1000 && id <= 2000){
								id_count++;
								printf("%d\n",id);
						}

						id = 0;
						memset(buf,0,sizeof(buf));
				}

				fclose(fp);
				unlink(argv[1]); //删除文件
				//打印获取的信息
				printf("parent: finished\n");
				printf("parent info: row_count=%d--id_count=%d\n",row_count,id_count);
		}
		return 0;
}

//提取每一行中的进程编号
int check_line(char* buf, int len){
		if(buf == NULL || len <= 0){
				return -2;
		}

		char* cur = buf;
		char* next = buf+1;
		char* pos1 = NULL; //pos1-->pid首个字符 
		char* pos2 = NULL; //pos2-->pid最后字符
		int flag = 0; //标志位,保证先获得pos1,再获得pos2
		int count = len - 1; //循环最大次数

		while(count--)
		{
				//算法:cur==空格 && next==数字 --> pos1 = next
				//cur==数字 && next==空格 --> pos2 = cur
				if((*cur == 32) && (*next >= '0' && *next <= '9')){
						pos1 = next;
						flag = 1;
				}
				if((*cur >= '0' && *cur <= '9') && (*next == 32)){
						if(flag == 1){
								pos2 = cur;
								break;
						}
				}
				//保证一行数据,一定会获得pos1和pos2,即break一定会触发
				cur = next;
				next = next + 1;
		}

		if(pos1!= NULL && pos2 != NULL){
				char buf2[32] = {0};
				int id = 0;
				//将目标字符段(pos1--pos2)提取到buf2中
				memcpy(buf2,pos1,(int)(pos2-pos1+1)); 
				buf2[strlen(buf2)] = '\n'; //添加一个结尾标示符'\n'
				sscanf(buf2,"%d\n",&id); //类型转换,i即为提取pid编号
				//				printf("In checkline: pos1=%p--pos2=%p\n",pos1,pos2);
				//				printf("In checkline: id=%d--buf2%s",id,buf2);
				return id;
		}
		else
		{
				//提取pid错误
				return -1;
		}
}

int check_line2(char* buf, int len){
		if(buf == NULL || len <= 0){
				return -2; //参数无效
		}
		int id = 0;
		char* s = strtok(buf," ");//截取第一列字段
		if(s != NULL){
				s = strtok(NULL," "); //截取第二列字段
				if(s != NULL){
						id = atoi(s);
						return id;
				}
				else{
						return -1; //获取失败,内容格式不正确
				}
		}
		else{
				return -1; //获取失败,内容格式不正确
		}
}
