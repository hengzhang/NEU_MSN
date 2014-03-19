/********************************************/
/*名称：m_sendMsg.c
/*描述： 1、维护发送的消息列表
	2、对发送消息列表进行各种操作

/*作者：张珩
/*日期：2011.6.28
/********************************************/
#ifndef __M_SENDMSG_
#define __M_SENDMSG_
#include <gtk/gtk.h>
#include <time.h>
#include "g_db.h"
#include "m_sendMsg.h"

//遍历函数传参用的结构体（功能简述）
/*
* foreachParm包括如下部分：
* func:遍历时进行操作的函数的函数指针
* parm:操作函数的参数
*/
typedef struct foreachParm_T{
    sendMsgFunc func;
    void *parm
}foreachParm;

static DB *sendMsgList = NULL;
static int doForeach(DB_DAT *sendMsgData,void *parm);
static int doSearch(LinpopSendMsg *sendMsg,void *serial);

/*******************************************************************/
/*
函数名:		initSendMsgList
功能:		初始化已发送消息链表
参数:		void
返回值:          void	
*/
/*****************************************************************/
void initSendMsgList(void){
    sendMsgList = db_new(NULL);
}

/*****************************************************************/
/*
函数名:		addSendMsg
功能:		根据参数列表的消息内容添加sendMsg
参数:		serial:序列号, user:用户名, machine:机器名, com:命令字, msg:消息内容, addr:来源地址
返回值:		void
*/
/*****************************************************************/
void addSendMsg(guint serial,gchar *user,gchar *machine,guint com,gchar *msg,struct sockaddr_in addr){
     LinpopSendMsg *sendMsg = g_malloc(sizeof(*sendMsg));
     sendMsg->user = g_strdup(user);
     sendMsg->machine = g_strdup(machine);
     sendMsg->com = com;
     sendMsg->msg = g_strdup(msg);
     sendMsg->to = addr;
     sendMsg->retry = 0;
     sendMsg->timeout_tag = -1;
     sendMsg->serial= serial;
     //将sendMsg添加到链表中
     sendMsg->db_tag = dbd_append(sendMsgList,sendMsg);
}

/*****************************************************************/
/*
函数名:		deleteSendMsgBySerial
功能:		根据序列号删除LinpopSendMsg
参数:		serial:序列号
返回值:		gboolean：TRUE——删除成功； FALSE——删除失败
*/
/*****************************************************************/
gboolean deleteSendMsgBySerial(guint serial)
{
    LinpopSendMsg *temp = NULL;
    //根据序列号找到指定的sendMsg
    foreachParm parm = {(sendMsgFunc)doSearch,(void *)&serial};
    //删除
    if((temp = (LinpopSendMsg *)db_foreach(sendMsgList,doForeach,&parm)) != 0)
    {
        dbd_free(sendMsgList,temp->db_tag);
        freeSendMsg(temp);
        return TRUE;
    }
    return FALSE;
}

/*****************************************************************/
/*
函数名:		getSendMsgBySerial
功能:		根据序列号获取LinpopSendMsg
参数:		serial:序列号, sendMsg:LinpopSendMsg *
返回值:		gboolean：TRUE——获取成功； FALSE——获取失败
*/
/*****************************************************************/
gboolean getSendMsgBySerial(guint serial,LinpopSendMsg *sendMsg)
{
    LinpopSendMsg *temp = NULL;
    //根据序列号找到指定的sendMsg
    foreachParm parm = {(sendMsgFunc)doSearch,(void *)&serial};
    //获取sendMsg
    if((temp = db_foreach(sendMsgList,doForeach,&parm)) != 0){
        sendMsg->db_tag = NULL;
        sendMsg->serial = temp->serial;
        sendMsg->to = temp->to;
        sendMsg->user = g_strdup(temp->user);
        sendMsg->machine = g_strdup(temp->machine);
        sendMsg->com = temp->com;
        sendMsg->msg = g_strdup(temp->msg);
        sendMsg->timeout_tag = temp->timeout_tag;
        sendMsg->retry = temp->retry;
        return TRUE;
    }
    else
        return FALSE;
}

/*****************************************************************/
/*
函数名:		setTimeoutTag
功能:		设置超时标记
参数:		serial:序列号, timeoutTag:超时标记，由gtk_timeout_add函数返回
返回值:		void
*/
/*****************************************************************/
void setTimeoutTag(guint serial,gint timeoutTag){
    LinpopSendMsg *temp = NULL;
    //根据序列号找到指定的sendMsg
    foreachParm parm = {(sendMsgFunc)doSearch,(void *)&serial};
    if((temp = db_foreach(sendMsgList,doForeach,&parm)) != 0)
        temp->timeout_tag = timeoutTag;
}

/*****************************************************************/
/*
函数名:		setRetry
功能:		设置重试次数
参数:		serial:序列号, retry:重试次数
返回值:		void
*/
/*****************************************************************/
void setRetry(guint serial,gint retry){
    LinpopSendMsg *temp = NULL;
    //根据序列号找到指定的sendMsg
    foreachParm parm = {(sendMsgFunc)doSearch,(void *)&serial};
    if((temp = db_foreach(sendMsgList,doForeach,&parm)) != 0)
        temp->retry= retry;
}

/*****************************************************************/
/*
函数名:		freeSendMsg
功能:		释放sendMsg所占用内存
参数:		sendMsg:LinpopSendMsg *
返回值:		void
*/
/*****************************************************************/
void freeSendMsg(LinpopSendMsg *sendMsg)
{
    g_free(sendMsg->user);
    g_free(sendMsg->machine);
    g_free(sendMsg->msg);
    g_free(sendMsg);
}

/*****************************************************************/
/*
函数名:		sendMsgListForeach
功能:		遍历sendMsgList链表
参数:		func:遍历函数, parm:遍历函数的参数
返回值:		int:遍历函数的返回值
*/
/*****************************************************************/
int sendMsgListForeach(sendMsgFunc func,void *arg){
    foreachParm parm = {func,arg};
    //遍历sendMsg链表
    return db_foreach(sendMsgList,doForeach,&parm);
}

/*****************************************************************/
/*
函数名:		doForeach
功能:		基本的遍历功能函数
参数:		sendMsgData:链表项, parm:foreachParm参数
返回值:		int:遍历函数的返回值
*/
/*****************************************************************/
static int doForeach(DB_DAT *sendMsgData,void *parm){
    LinpopSendMsg *sendMsg = (LinpopSendMsg *)sendMsgData->data;
    sendMsgFunc func = ((foreachParm *)parm)->func;
    void *arg = ((foreachParm *)parm)->parm;
    return func(sendMsg,arg);
}

/*****************************************************************/
/*
函数名:		doSearch
功能:		基本的搜索功能函数
参数:		sendMsg:LinpopSendMsg *, serial:序列号
返回值:		int:返回搜索的结果
*/
/*****************************************************************/
static int doSearch(LinpopSendMsg *sendMsg,void *serial){
    if(sendMsg->serial == *(guint *)serial)
        return (int)sendMsg;
    else
        return 0;
}
#endif
