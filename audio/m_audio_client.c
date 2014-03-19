#include "m_audio_client.h"


//根据ip和端口号连接TCP
int connectTCP(char *ip, int port)
{

//	int send_sockfd;
	struct sockaddr_in pin_addr;
	bzero(&pin_addr, sizeof(pin_addr));
	pin_addr.sin_family = AF_INET;
	//pin_addr.sin_addr.s_addr = htonl( INADDR_ANY);
	pin_addr.sin_addr.s_addr = inet_addr(ip);
	pin_addr.sin_port = htons(port);

	//创建socket
	if ((send_sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket");
		return -1;
	}

	//连接TCP
	if (connect(send_sockfd, (void*) &pin_addr, sizeof(pin_addr)) == -1)
	{
		perror("connect");
		return -1;
	}

	return 0;
}


