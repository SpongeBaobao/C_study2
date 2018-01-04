#include <stdio.h>
#include <time.h>
#include <stdlib.h>

//随机数函数
int main()
{
//srand((unsigned int) time(NULL));

int i;

for(i = 0; i < 10; i++)
{
printf("%d\n",rand());
}

return 0;
}
