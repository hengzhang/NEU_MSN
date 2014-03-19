
/********************************************/
/*名称：m_broadcast.h
/*描述：广播地址的定义
/*作者：张珩
/*日期：2011.6.28
/********************************************/

#ifndef M_BROADCAST_H
#define M_BROADCAST_H
#include <netinet/in.h>
#include <gtk/gtk.h>
#include "g_db.h"

/****************************************************/
/*
函数名:		broadcastFunc
功能：		函数指针
参数：		struct in_addr *addr：ip地址指针, void *parm：函数参数
返回值:		 int
*/
/**************************************************/
typedef int (*broadcastFunc)(struct in_addr *addr,void *parm);//______

void initBroadcastList(void);
void setBroadcasts(char *lbcast,char *bcasts);
int broadcastListForeach(broadcastFunc func,void *parm);
/*#ifndef __M_BROADCAST_
extern DB *broadcastList;
#endif*/
#endif
