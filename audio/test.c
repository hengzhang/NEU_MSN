#include "pthread.h"
#include "malloc.h"

#include "m_audio.h"


int main()
{
	
	tcpInfo *tcp = (tcpInfo*)malloc(sizeof(tcpInfo)); 
	pthread_t audioThread;
	int res;
	
	tcp->ip = "10.10.31.45";
	tcp->port = 5559;

	res = pthread_create(&audio_thread, NULL, speechChat, tcp);
	if(0 != res)
	{
		perror("create thread");
		return -1;
	}
	//speechChat(tcp);
	

	return 0;
}
