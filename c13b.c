#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//存储符号的位置和权重
typedef struct _OPVALUE
{
		int pos;
		int value;
}opv;

//计算字符串中的运算符个数
int op_num(char* str, int len)
{
		int i;
		int num = 0;
		for(i = 0; i < len; i++){
				if(str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/')
						num++;
		}

		return num;
}

//扫描字符串,确定运算符位置和权重
int see_str(char* str, int len, opv** pout)
{
		int num = op_num(str,len);
		*pout = (opv*)malloc(sizeof(opv)*num);

		int base_value = 1;
		int i;
		int j = 0;
		for(i = 0; i<len; i++){
				if(str[i] == '('){ base_value+=5;}
				if(str[i] == ')'){ base_value-=5;}

				if(str[i] == '+' || str[i] == '-'){
						(*pout)[j].pos = i;
						(*pout)[j].value = base_value;
						j++;
				}

				if(str[i] == '*' || str[i] == '/'){
						(*pout)[j].pos = i;
						(*pout)[j].value = base_value + 1;
						j++;
				}
		}
		return 0;
}

//对运算符结构体数组进行权重由大到小的稳定排序
void sort_opv(opv* oparr, int len)
{
		int i,j;
		for(i = 0; i < len-1; i++){
				for(j = 0; j < len-1-i; j++){
						if(oparr[j].value > oparr[j+1].value){
								opv temp = {0};
								temp = oparr[j];
								oparr[j] = oparr[j+1];
								oparr[j+1] = temp;
						}
				}
		}
}

//字符串中缀转换后缀函数
void trans(char* arr, int len, char** out, opv* oparr, int num){
		char buf[1024] = {0};
		strncpy(buf,arr,len);

		int i;
		int j = 0;
		*out = (char*)malloc(sizeof(char)*len+1);

		for(i =0; i<num; i++){
				if(buf[oparr[i].pos - 1] != '\0'  && buf[oparr[i].pos - 1] != '(' && buf[oparr[i].pos - 1] != ')'){
						(*out)[j] = buf[oparr[i].pos - 1];
						j++;
						buf[oparr[i].pos - 1] = '\0';
				}

				if(buf[oparr[i].pos + 1] != '\0' && buf[oparr[i].pos + 1] != '(' && buf[oparr[i].pos + 1] != ')'){
						(*out)[j] = buf[oparr[i].pos + 1];
						j++;
						buf[oparr[i].pos + 1] = '\0';
				}

				if(buf[oparr[i].pos] != '\0' && buf[oparr[i].pos] != '(' && buf[oparr[i].pos] != ')'){
						(*out)[j] = buf[oparr[i].pos];
						j++;
						buf[oparr[i].pos] = '\0';
				}
		}

		return;
}
int main()
{
		char str[1024] = "8+(3-1)*5";
		char ret[1024] = {0};
		int num = op_num(str,strlen(str));
		opv* p_oparr = NULL;
		see_str(str,strlen(str),&p_oparr);
		sort_opv(p_oparr,num);

		int i;
		char* ret_out=NULL;
		for(i=0; i<num; i++){
				//p_oparr[i].pos进行整理
				trans(str, strlen(str), &ret_out, p_oparr, num);
		}
		strncpy(ret,ret_out,strlen(str));

		printf("中缀表达式:%s\n",str);
		printf("后缀表达式:%s\n",ret);

		return 0;
}
