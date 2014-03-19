/********************************************/
/*名称：filetansfer.c
/*描述：此文件定义了文件传输相关的函数
/*作者：
/*日期：
/********************************************/

#include <string.h>
#include <time.h>
/*
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <fcntl.h>
*/

#include "filetansfer.h"


/**************************************************/
/*名称：file_send
/*描述：发送文件函数
/*作成日期：
/*参数：
         参数1：file_info、file_pthread_t *、输入参数、传输文件的相关信息，其中包括文件名称和接收方的地址信息
/*返回值：void
/*作者：
/***************************************************/
int file_send(file_pthread_t * file_info)
{	
	int rc,file_send,r_size;
	//struct sockaddr_in addr;/*测试用socket结构体*/
	int buffer[512];
	//要传输的文件的描述符号
	int fp;

	char *path = file_info->path;
	
        fp = open(path,O_RDONLY);

	file_send = socket(AF_INET,SOCK_STREAM,0);

	if(file_send < 0)
		die("file_send");
       /*测试代码*/
      /*memset(&addr,0,sizeof(struct sockaddr_in));	
	addr = file_info->client ;
	addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr("192.168.16.128");
        addr.sin_port = htons(9734);*/
        rc = connect(file_send,(struct sockaddr *)&(file_info->client),sizeof(struct sockaddr));
	if(rc < 0){
		die("connect failed");
		return 0 ;/*修改测试*/
	}
        
        else
	{		
		while((r_size=read(fp,buffer,sizeof(buffer)))!=-1&&r_size>0){
			if((send(file_send,buffer,r_size,MSG_OOB))!=r_size){
				printf("send error");
				return 0 ;
			}
   		} 
	}
	//test
	printf("init file_send socket successful!\n");
	printf("file send start\n");

	//test
	printf("file send end\n");
	return 1 ;
}


/**************************************************/
/*名称：file_recv
/*描述：接收文件函数
/*作成日期： 
/*参数：
         参数1：client、struct sockaddr_in 、输入参数、发送文件方的网络地址信息
         参数2：file_name、char *、输入参数、文件名称
/*返回值：void
/*作者：
/***************************************************/
//void file_recv(struct sockaddr_in client,char *file_name)
void file_recv(void* argu)
{
	int rc,file_recv;
	pid_t pid ;
	struct sockaddr_in addr;
	//要接收的文件的描述符号	
	int  fp;
	struct tm *newtime ;
	char temp[40];
        char *file_name = NULL;
	time_t longtime ;
	printf("begin create server !\n");

	int addLen = sizeof(struct sockaddr_in);

	file_recv = socket(AF_INET,SOCK_STREAM,0);
	printf("socket start !\n");
	if(file_recv < 0){
		die("file_recv");
		return 0 ;
	}
	memset(&addr,0,sizeof(struct sockaddr_in));
	
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(9734);

	
        if(bind(file_recv,(struct sockaddr*)&addr,sizeof(struct sockaddr_in)))
		die("bind");

	printf("bind finish\n");
	
	if(listen(file_recv,5) < 0)
		die("listen");
	
	printf("listen finish,begin accept\n");
	/*获取接收信息的套接字*/
	rc = accept (file_recv,(struct sockaddr*)&addr,&addLen);
	if(rc < 0)
		die("accept");
	//test
	else{
	pid = fork();
		if(pid==0){
			printf("init file_recv socket successful!\n");
			printf("file recv start\n");
			time(&longtime);
			newtime = localtime(&longtime);
			strftime(temp,40,"%Y%m%d%H%M%S",newtime);
			file_name =  temp;
			fp = open(file_name,O_WRONLY | O_CREAT,00700);
			if(fp < 0)
			{
				die("open recv file failed");
			}
			else copyData(rc,fp);
		}
	}
	//test
	wait();
	printf("file recv end\n");
}
