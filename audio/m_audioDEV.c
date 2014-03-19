#include "m_audioDEV.h"


//打开设备
int openDEV(char *path, int mode)
{
	if(READDEV == mode)
	{
		fr = open(path, O_RDONLY);
		if(fr == -1)
		{
			perror("read open");
			return -1;
		}
	}
	else if(WRITEDEV == mode)
	{
		fw = open(path, O_WRONLY);
		if(fw == -1)
		{
			perror("write open");
			return -1;
		}
	}
	else
	{
		printf("mode is wrong\n");
		return -1;
	} 

	return 0;
}


//设置设备参数
int setAudioArg(int fd)
{
	//参数变量和设置参数状态
	int arg, status;

	//设置size
	arg = SIZE;                                        
	status = ioctl(fd, SOUND_PCM_WRITE_BITS, &arg);    
	if (status == -1)
	{                                  
		perror("SOUND_PCM_WRITE_BITS ioctl failed");
		return -1;     
	}
	if (arg != SIZE)
	{                                   
		perror("unable to set sample size");
		return -1;	
	}             
		     
	//设置channle      
	arg = CHANNELS;                                    
	status = ioctl(fr, SOUND_PCM_WRITE_CHANNELS, &arg);
	if (status == -1)                                  
	{
		perror("SOUND_PCM_WRITE_CHANNELS ioctl failed"); 
		return -1;
	}
	if (arg != CHANNELS)                               
	{
		perror("unable to set number of channels");
		return -1;
	}      
		       
	//设置rate        
	arg = RATE;                                        
	status = ioctl(fr, SOUND_PCM_WRITE_RATE, &arg);    
	if (status == -1)                                  
	{
		perror("SOUND_PCM_WRITE_WRITE ioctl failed");
		return -1;
	}
	if(arg != RATE)
	{
		perror("unable to set rate");
		return -1;
	}  

	return 0;
}

//读取设备，相当于录音
void readDEV(unsigned char* buf)
{
	//读取设备状态
	int status;

	printf("Say something:\n");
	//sem_wait(&sem);
	//读取设备                      
	status = read(fr, (char*)buf, BUFSIZE);
	//sem_post(&sem);
	printf("%d\n", status);
	if (status != BUFSIZE)                       
	  perror("read wrong number of bytes");      
}

//写设备，相当于放音
void writeDEV(unsigned char* buf)
{
	//写设备状态
	int status; 

	printf("you said:\n");
	//sem_wait(&sem);
	//写设备                      
	status = write(fw, (char*)buf, BUFSIZE);
	//sem_post(&sem);
	printf("%d\n", status);
	if (status != BUFSIZE)                       
	  perror("write wrong number of bytes");      
}



