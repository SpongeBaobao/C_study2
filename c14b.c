#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//1
/*
   void main(){
   FILE* fp_write= NULL;
   fp_write = fopen("./mydata.txt", "w");
   if (fp_write == NULL){
   perror("fopen:");
   return;
   }
   fprintf(fp_write,"hello world:%d!",10);

   fclose(fp_write);

   FILE* fp_read = NULL;
   fp_read = fopen("./mydata.txt", "rb");
   if (fp_read == NULL){
   perror("fopen:");
   return;
   }
   char temps[1024] = { 0 };
   while (!feof(fp_read)){
   fscanf(fp_read, "%s", temps);
   printf("%s", temps);
   }
   fclose(fp_read);
   }
   */

//2 读取配置文件(key:value)形式
int main(int argc, char* argv[])
{
		//1检查参数合法性
		if(argc != 2){
				fprintf(stderr,"format:<%s> <filename>\n",argv[0]);
				return -1;
		}

		//2打开文件
		FILE* fp = fopen(argv[1],"r");
		if(fp == NULL){
				printf("fopen failure\n");
				return -2;
		}
		//3读取文件:行读取
		char buf[1024] = {0};
		while(!feof(fp)){
				memset(buf,0,1024);
				if(fgets(buf,1024,fp) == NULL){
						printf("读取结束\n");
						return 0;
				}

				//忽略注释行
				if(buf[0] == '#'){
						continue;
				}

				//4解析行信息
				char key[1024] = {0};
				char value[1024] = {0};

				char* pos = strchr(buf,':');
				strncpy(key,buf,pos-buf);

				char* pend = &buf[strlen(buf)-2]; //指向最后一个非空格字符
				strncpy(value,pos+1,pend-pos);

				/*
				   int n = strlen(buf)-(pos-buf+2);
				   strncpy(value,pos+1,n);
				   */

				printf("key:%s  value:%s\n",key,value);
		}
		//5关闭文件
		fclose(fp);

		return 0;
}
