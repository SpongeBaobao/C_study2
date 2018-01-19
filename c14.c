/*
#include <sys/types.h>
#include <sys/stat.h> 
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//1 创建文件,写入内容,文件重命名,截断文件,读取文件,删除文件
int main(int argc, char* argv[])
{
int ret, fd;
char buf[1024] = "hello world";

//1创建文件,并写入内容
ret = access(argv[1],F_OK);
if(ret == 0){
//目标文件存在
printf("目标文件存在\n");
fd = open(argv[1],O_RDWR|O_APPEND, 0777);
}else{
//目标文件不存在
printf("目标文件不存在\n");
fd = open(argv[1],O_RDWR|O_CREAT,0777);
}

ret = write(fd,buf,strlen(buf));
if(ret > 0){
printf("写入文件成功\n");
}

//2关闭该文件
lseek(fd,0,SEEK_SET);
ret = close(fd);
if(ret == 0){
printf("文件关闭成功\n");
}

//3文件重命名
ret = rename(argv[1],"newfile.txt");
if(ret == 0){
printf("文件重命名成功\n");
}

//4截断文件
struct stat st = {0};
int len;
ret = stat("newfile.txt",&st);
if(ret == 0){
printf("文件属性信息获取成功\n");
printf("文件长度:%d\n",st.st_size);
len = st.st_size / 2; //原文件的一半
}

ret = truncate("newfile.txt",len);
if(ret == 0){
printf("文件截取成功\n");
}

//5读取文件
memset(buf,0,sizeof(buf));
fd = open("newfile.txt",O_RDONLY);
if(fd > 0){
ret = read(fd,buf,sizeof(buf));
if(ret > 0){
printf("读取文件成功\n");
printf("文件内容:%s\n",buf);
}
}

//6删除文件
close(fd);
ret = unlink("newfile.txt");
if(ret == 0){
		printf("文件删除成功\n");
}

return 0;
}
*/

//目录操作函数
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

//遍历一个目录下的所有文件和类型和大小
int main(int argc, char* argv[])
{
		int ret;

		//判断目录是否有效
		ret = access(argv[1],F_OK);
		if(ret != 0){
				printf("目标路径无效\n");
				return -1;
		}

		//打开目录
		DIR* dp = NULL;
		dp = opendir(argv[1]);
		if(dp){
				printf("目录打开成功\n");
		}
		else{
				return -2;
		}

		//读取目录下的信息
		char filetype[1024] = {0};
		char path[1024] = {0};
		struct dirent* p_dir = NULL;
		struct stat st;

		while((p_dir = readdir(dp)) != NULL){
				//打印文件名,文件类型,文件大小
				printf("%8s  ",p_dir->d_name);

				memset(filetype,0,sizeof(filetype));
				switch(p_dir->d_type)
				{
						case DT_BLK  : strcpy(filetype, "block device");   		break;
						case DT_CHR  : strcpy(filetype, "character device"); 	break;
						case DT_DIR  : strcpy(filetype, "directory"); 			break;
						case DT_FIFO : strcpy(filetype, "pipe/FIFO"); 			break;
						case DT_LNK  : strcpy(filetype, "symbolic link");		break;
						case DT_REG  : strcpy(filetype, "regular"); 			break;
						case DT_SOCK : strcpy(filetype, "UNIX domain socket");  break;
						default      : strcpy(filetype, "unknown"); 		    break;
				}
				printf("%20s  ",filetype);

				//获取子文件大小
				memset(path,0,sizeof(path));
				memset(&st,0,sizeof(struct stat));
				strcpy(path,argv[1]);
				strcat(path,"/");
				strcat(path,p_dir->d_name);
				ret = stat(path,&st);
				if(ret == 0){
						printf("%lu\n",st.st_size);
				}
				else
				{
						printf("size err\n");
				}

		}
		return 0;
}
