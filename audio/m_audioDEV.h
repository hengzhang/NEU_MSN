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

#define READDEV 0
#define WRITEDEV 1
                    
#define LENGTH 3                      
#define RATE 8000                 
#define SIZE 8                       
#define CHANNELS 2 
#define BUFSIZE 24000



int fr, fw;

int openDEV(char *path, int mode);
int setAudioArg(int fd);
void readDEV(unsigned char* buf);
void writeDEV(unsigned char* buf);


