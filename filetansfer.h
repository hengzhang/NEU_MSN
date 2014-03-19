/********************************************/
/*名称：filetansfer.h
/*描述： 有关文件传输的函数声明
/*作者：
/*日期：
/********************************************/
#ifndef __I_FILETANSFER_H
#define __I_FILETANSFER_H

#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <fcntl.h>

typedef struct file_argu{
	char *path;
	struct sockaddr_in client;//本地网络信息
}file_pthread_t;



int file_send(file_pthread_t * file_info);
//void file_recv(struct sockaddr_in client,char *file_name);
void file_recv(void* argu);

#endif
