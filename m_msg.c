/********************************************/
/*名称：m_msg.h
/*描述： 定义消息的结构和进行的操作


/*作者：张珩
/*日期：2011.6.28
/********************************************/
#ifndef M_MSG_H
#define M_MSG_H

//消息结构体的定义（结构体功能简述）
/*
* LinpopMessage（结构体名称）包括如下部分：
* from:来源IP
* fromlen:消息长度
* serial:消息序列号
* user:用户名
* machine:用户主机名
* group:用户组
* com:消息命令
* com2:消息附加命令
* appendix:消息内容
*/
typedef struct LinpopMessage_T
{
  struct sockaddr_in from;
  gint fromlen;
  gchar *serial;
  gchar *user;
  gchar *machine;
  gchar *group;
  guint com, com2;
  gchar *appendix;
} LinpopMessage;

void freeMsg(LinpopMessage *msg);
void printMsg(LinpopMessage *msg);
#endif
