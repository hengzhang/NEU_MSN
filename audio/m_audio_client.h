#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <unistd.h>
//#include <pthread.h>
#include <signal.h>                                  
#include <fcntl.h>                                   
#include <sys/types.h>                               
#include <sys/ioctl.h>                                                                                               
#include <linux/soundcard.h>  

//#include <semaphore.h>


int send_sockfd;

int connectTCP(char *ip, int port);

