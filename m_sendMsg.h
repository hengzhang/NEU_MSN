/********************************************/
/*名称：m_sendMsg.h
/*描述： 1、______
	2、______

/*作者：______
/*日期：______
/********************************************/
#ifndef M_SENDMSG_H
#define M_SENDMSG_H
#include <gtk/gtk.h>
#include "g_db.h"
#include <netinet/in.h>

//______（结构体功能简述）
/*
* ______（结构体名称）包括如下部分：
* db_tag:______
* serial:______
* to:______
* user:______
* machine:______
* com:______
* msg:______
* timeout_tag:______
* retry:______
*/
typedef struct LinpopSendMsg_T
{
  struct DB_DAT_T *db_tag;
  guint serial;
  struct sockaddr_in to;
  gchar *user;
  gchar *machine;
  guint com;
  gchar *msg;
  gint timeout_tag;
  gint retry;
} LinpopSendMsg;

//重定义指针函数
/*******************************************************************/
/*
函数名:		sendMsgFunc
功能:		______
参数:		sendMsg:______, parm:______
返回值:          int:______	
*/
/*****************************************************************/
typedef int (*sendMsgFunc)(LinpopSendMsg *sendMsg,void *parm);

void initSendMsgList(void);
void addSendMsg(guint serial,gchar *user,gchar *machine,guint com,gchar *msg,struct sockaddr_in addr);
gboolean deleteSendMsgBySerial(guint serial);
gboolean getSendMsgBySerial(guint serial,LinpopSendMsg *sendMsg);
void setTimeoutTag(guint serial,gint timeoutTag);
void setRetry(guint serial,gint retry);
int sendMsgListForeach(sendMsgFunc func,void *parm);
void freeSendMsg(LinpopSendMsg *sendMsg);

#endif
