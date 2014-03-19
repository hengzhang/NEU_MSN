#include "m_audio_server.h"


//根据ip和端口号创建TCP
int createTCP(char *ip, int port)
{
	

	int recv_sockfd;
	struct sockaddr_in sin_addr, pin_addr;
	int len, pin_addr_size;
	memset(&sin_addr, 0 , sizeof(sin_addr));
	sin_addr.sin_family = AF_INET;
	//sin_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	sin_addr.sin_addr.s_addr = inet_addr(ip);
	sin_addr.sin_port = htons(port);

	//创建socket
	if ((recv_sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket");
		//exit(1);
		return -1;
	}

	//帮定
	if (bind(recv_sockfd, (struct sockaddr*) &sin_addr, sizeof(sin_addr)) < 0)
	{
		perror("bind");
		//exit(1);
		return -1;
	}

	//监听
	if (listen(recv_sockfd, 5) < 0)
	{
		perror("listen");
		//exit(1);
		return -1;
	}

	//接收连接
	if ((new_sockfd = accept(recv_sockfd, (struct sockaddr *) &pin_addr,&pin_addr_size)) < 0)
	{
		perror("accept");
		//exit(1);
		return -1;
	}

	return 0;
}

