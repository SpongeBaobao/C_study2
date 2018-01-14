#include <sys/types.h>
#include <sys/stat.h> 
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//系统调用函数,文件操作
int main(int argc, char* argv[])
{
		if(argc < 2)
		{
				fprintf(stderr,"Usage:%s <filename>.\n",argv[0]);
				return -1;
		}

		int fd;
		//打开一个文件:若不存在则创建之,反之则追加写入
		fd = open(argv[1],O_RDWR|O_CREAT|O_APPEND, 0777);
		if(fd < 0){
				perror("open failure");
				return -2;
		}

		//读取该文件
		char buf[1024] = "hello world";
		int count = write(fd,buf,strlen(buf));
		if(count > 0){
				printf("write success\n");
		}
		else{
				printf("write file error\n");
				return -2;
		}

		//将该文件光标置首
		lseek(fd,0,SEEK_SET);

		//读取文件中的数据
		memset(buf,0,sizeof(buf));
		count = read(fd,buf,sizeof(buf));
		if(count > 0){
				printf("read:%s\n",buf);
		}
		else{
				printf("read file error\n");
				return -2;
		}

		//获取文件的字节数
		lseek(fd,0,SEEK_END);
		count = 0;
		while(lseek(fd,-1,SEEK_CUR) > 0){
				count++;
		}
		printf("file byte is:%d\n",count+1);

		//关闭该文件
		close(fd);

		return 0;
}
