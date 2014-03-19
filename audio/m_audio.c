#include "m_audio.h"

//语音聊天
int speechChat(tcpInfo *tcp)
{
	//建立TCP连接的ip和端口号
	char *ip = tcp->ip;
	int port = tcp->port;

	//打开设备标识符
	int fd;
	//打开设备状态
	int status;

	//打开dsp设备，用于录音和放音
	fd = open("/dev/dsp", O_RDWR);
	//设置dsp参数
	setAudioArg(fd);
	//留作以后扩展为多线程使用
	fr = fd;
	fw = fd;

	//如果该终端为服务断，则建立TCP连接，并监听
	if(SERVER)
	{
		//创建TCP
		status = createTCP(ip, port);
		if(status == -1)
		{
			printf("create tcp failed\n");
			exit(1);
		}

		//服务端的录音和放音
		speechServer();
		
		//关闭TCP连接
		close(new_sockfd);
	}
	//如果该终端为客户端，则根据ip和端口号连接TCP
	else
	{
		//连接TCP
		status = connectTCP(ip, port);
		if(status == -1)
		{
			printf("connect tcp failed\n");
			exit(1);
		}

		//客户端的录音和放音
		speechClient();

		//关闭TCP连接
		close(send_sockfd);
	}

/*	fr = open("/dev/dsp", 0);
	setAudioArg(fr);
	fw = open("/dev/dsp", 1);
	setAudioArg(fw);
*/
	return 0;
}//end speechChat


//服务端录音和放音
void speechServer()
{
	//不停的录音和放音，直到TCP连接断开
	while (1)
	{
		//接收客户端发来的数据
		if (recv(new_sockfd, recvbuf, BUFSIZE, 0) == -1)
		{
			perror("recv");
			exit(1);
		}		
		//将接收的数据写入到设备中，即放音
		writeDEV(recvbuf);
		sleep(2);

		//录音
		readDEV(sendbuf);
		//将数据发送到客户端
		if (send(new_sockfd, sendbuf, BUFSIZE, 0) == -1)
		{
			perror("send");
			exit(1);
		}
	}
}


//客户端的录音和放音
void speechClient()
{
	//不停的录音和放音
	while (1)
	{
		//录音
		readDEV(sendbuf);
		//将数据发送给服务端
		if (send(send_sockfd, sendbuf, BUFSIZE, 0) == -1)
		{
			perror("send");
			exit(1);
		}

		sleep(2);
		//从服务端接收数据
		if (recv(send_sockfd, recvbuf, BUFSIZE, 0) == -1)
		{
			perror("recv");
			exit(1);
		}		
		//将接收的数据写入到设备中
		writeDEV(recvbuf);

	}

}

