#include <stdio.h>
typedef struct date//定义数据类型，无typedef是定义变量
{
    int year;
    int month;
    int day;
}Date;
int main()
{
    Date today;
    scanf("%d%d%d",&today.year,&today.month,&today.day);
    printf("%d %d %d",today.year,today.month,today.day);
    return 0;
} 
