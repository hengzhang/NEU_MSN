#include "m_audio_server.h"
#include "m_audio_client.h"
#include "m_audioDEV.h"

//#define SERVER 0
int SERVER = 0;

typedef struct
{
	char *ip;
	int port;
}tcpInfo;

unsigned char sendbuf[LENGTH*RATE*SIZE*CHANNELS/8];
unsigned char recvbuf[LENGTH*RATE*SIZE*CHANNELS/8];

int speechChat(tcpInfo *tcp);

void speechServer(void);

void speechClient(void);

