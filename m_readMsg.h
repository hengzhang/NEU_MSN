/********************************************/
/*名称：m_readMsg.h
/*描述： 1、维护接收到的消息链表
	2、对接收到的消息链表进行各种操作

/*作者：张珩
/*日期：2011.6.28
/********************************************/
#ifndef M_READMSG_H
#define M_READMSG_H
#include <netinet/in.h>
#include <gtk/gtk.h>
#include "g_db.h"

//接收到的消息的结构体（结构体功能简述）
/*
* LinpopReadMsg（结构体名称）包括如下部分：
* db_tag:指向所在链表项的指针
* from:来源IP
* serial:序列号
* msg:消息内容
* win:显示到的窗体，NULL表示没有窗体
* have_url:消息中是否包含Url
* shown:消息是否已经显示
*/
typedef struct LinpopReadMsg_T
{
  struct DB_DAT_T *db_tag;
  struct sockaddr_in from;
  guint serial;
  gchar *msg;
  GtkWidget *win;
  unsigned have_url:1;
  unsigned shown:1;
} LinpopReadMsg;

/*******************************************************************/
/*
定义名:		readMsgFunc
功能:		对LinpopReadMsg进行遍历的函数的定义名称
参数:		readMsg:LinpopReadMsg, parm:函数参数
返回值:          int:返回的数据
*/
/*****************************************************************/
typedef int (*readMsgFunc)(LinpopReadMsg *readMsg,void *parm);
void initReadMsgList(void);
void addReadMsg(gchar *msg,guint serial,struct sockaddr_in from);
gboolean deleteReadMsgBySerial(guint serial);
gboolean getReadMsgBySerial(guint serial,LinpopReadMsg *readMsg);
void setGtkWidget(guint serial,GtkWidget *win);
void setUrlFlag(guint serial,unsigned urlFlag);
void setShownFlag(guint serial,unsigned shownFlag);
int readMsgListForeach(readMsgFunc func,void *parm);
void freeReadMsg(LinpopReadMsg *readMsg);

#endif
