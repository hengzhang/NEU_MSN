#include "mytime.h"
/************************************************/
//该文件定义了获取指定格式的系统时间的函数
/************************************************/
/*************************************************/
/*
函数名：	void getCurrentDay(char buf[],size)
参数：		buf:用于存放时间的缓存区，size：该缓存区的大小
返回置：	void
功能：		获取当前的系统时间，格式为20100701
*/
/***********************************************/

void getCurrentDay(char  buf[],int size)
{
	time_t timep;
	struct tm * p;
	time(&timep);
	p = localtime(&timep);
	strftime(buf,size,"%Y%m%d",p);
}
/*************************************************/
/*
函数名：	void getCurrentTime(char buf[],size)
参数：		buf:用于存放时间的缓存区，size：该缓存区的大小
返回置：	void
功能：		获取当前的系统时间，格式为2010-07-01    18:12:56
*/
/***********************************************/
void getCurrentTime(char buf[],int size)
{
	time_t timep;
	struct tm * p;
	time(&timep);
	p = localtime(&timep);
	strftime(buf,size,"%Y-%m-%d    %H:%M:%S",p);
	printf("%s\n",buf);
}	


