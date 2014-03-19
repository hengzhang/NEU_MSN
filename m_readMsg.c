/********************************************/
/*名称：m_readMsg.c
/*描述： 1、维护接收到的消息链表
	2、对接收到的消息链表进行各种操作

/*作者：张珩
/*日期：2011.6.28
/********************************************/
#ifndef __M_READMSG_
#define __M_READMSG_
#include "m_readMsg.h"
static DB *readMsgList = NULL;

//进行遍历时用来临时传递参数的结构体（功能简述）
/*
* foreachParm包括如下部分：
* func:函数指针
* parm:函数参数
*/
typedef struct foreachParm_T{
    readMsgFunc func;
    void *parm;
}foreachParm;

static int doForeach(DB_DAT *readMsgData,void *parm);
static int doSearch(LinpopReadMsg *readMsg,void *serial);

/*******************************************************************/
/*
函数名:		initReadMsgList
功能:		初始化 readMsgList
参数:		void
返回值:          void	
*/
/*****************************************************************/
void initReadMsgList(void)
{
    readMsgList = db_new(NULL);
}

/*****************************************************************/
/*
函数名:		addReadMsg
功能:		增加readMsg
参数:		msg:LinpopReadMsg, serial:序列号, from:来源IP
返回值:		void
*/
/*****************************************************************/
void addReadMsg(gchar *msg,guint serial,struct sockaddr_in from){
    LinpopReadMsg *readMsg = g_malloc(sizeof(*readMsg));
    //设置readMsg的各个成员
    readMsg->msg = msg;
    readMsg->serial = serial;
    readMsg->from = from;
    readMsg->win = NULL;
    readMsg->have_url = 0;
    readMsg->shown= 0;
    readMsg->db_tag = dbd_append(readMsgList,readMsg);
}

/*****************************************************************/
/*
函数名:		deleteReadMsgBySerial
功能:		根据序列号删除
参数:		serial:序列号
返回值:		gboolean：TRUE——删除成功； FALSE——删除失败
*/
/*****************************************************************/
gboolean deleteReadMsgBySerial(guint serial){
    LinpopReadMsg *temp = NULL;
    //根据序列号寻找readMsg
    foreachParm parm = {(readMsgFunc)doSearch,(void *)&serial};
    //删除readMsg
    if((temp = db_foreach(readMsgList,doForeach,&parm)) != 0){
        dbd_free(readMsgList,temp->db_tag);
        freeReadMsg(temp);
        return TRUE;
    }
    else 
        return FALSE;
}

/*****************************************************************/
/*
函数名:		getReadMsgBySerial
功能:		根据序列号获取
参数:		serial:序列号, readMsg:LinpopReadMsg *
返回值:		gboolean：TRUE——获取成功； FALSE——获取失败
*/
/*****************************************************************/
gboolean getReadMsgBySerial(guint serial,LinpopReadMsg *readMsg){
     LinpopReadMsg *temp = NULL;
     //根据序列号找到指定的readMsg
     foreachParm parm = {(readMsgFunc)doSearch,(void *)&serial};
     //获取readMsg
     if((temp = db_foreach(readMsgList,doForeach,&parm)) != 0){
         readMsg->db_tag = NULL;
         readMsg->from = temp->from;
         readMsg->serial = temp->serial;
         readMsg->msg = g_strdup(temp->msg);
         readMsg->win = temp->win;
         readMsg->have_url = temp->have_url;
         readMsg->shown = temp->shown;
         return TRUE;
     }
     else 
        return FALSE;
}

/*****************************************************************/
/*
函数名:		setGtkWidget
功能:		设置显示窗体
参数:		serial:序列号, win:新的显示窗体，NULL代表无窗体
返回值:		void
*/
/*****************************************************************/
void setGtkWidget(guint serial,GtkWidget *widget){
     LinpopReadMsg *temp = NULL;
     //根据序列号找到指定的readMsg
     foreachParm parm = {(readMsgFunc)doSearch,(void *)&serial};
     if((temp = db_foreach(readMsgList,doForeach,&parm)) != 0)
         temp->win = widget;
}

/*****************************************************************/
/*
函数名:		setUrlFlag
功能:		设置是否有url
参数:		serial:序列号, urlFlag:url标志
返回值:		void
*/
/*****************************************************************/
void setUrlFlag(guint serial,unsigned urlFlag){
     LinpopReadMsg *temp = NULL;
     //根据序列号找到指定的readMsg
     foreachParm parm = {(readMsgFunc)doSearch,(void *)&serial};
     if((temp = db_foreach(readMsgList,doForeach,&parm)) != 0)
         temp->have_url = urlFlag;
}

/*****************************************************************/
/*
函数名:		setShownFlag
功能:		设置是否已经显示的标志
参数:		serial:序列号, shownFlag:是否已经显示的标志
返回值:		void
*/
/*****************************************************************/
void setShownFlag(guint serial,unsigned shownFlag){
     LinpopReadMsg *temp = NULL;
     //根据序列号找到指定的readMsg
     foreachParm parm = {(readMsgFunc)doSearch,(void *)&serial};
     if((temp = db_foreach(readMsgList,doForeach,&parm)) != 0)
         temp->shown= shownFlag;
}

/*****************************************************************/
/*
函数名:		readMsgListForeach
功能:		遍历整个readMsg链表的函数
参数:		func:进行遍历时的操作函数, parm:操作函数的参数
返回值:		int:func函数的返回值
*/
/*****************************************************************/
int readMsgListForeach(readMsgFunc func,void *arg){
     foreachParm parm = {func,arg};
     return db_foreach(readMsgList,doForeach,&parm);
}

/*****************************************************************/
/*
函数名:		doForeach
功能:		基本的遍历函数
参数:		readMsgData:readMsgList链表的数据项, parm:传入的foreachPram类型的参数
返回值:		int:遍历函数的返回值
*/
/*****************************************************************/
static int doForeach(DB_DAT *readMsgData,void *parm){
    LinpopReadMsg *readMsg = (LinpopReadMsg *)readMsgData->data;
    readMsgFunc func = ((foreachParm *)parm)->func;
    void *arg= ((foreachParm *)parm)->parm;
    return func(readMsg,arg);
}

/*****************************************************************/
/*
函数名:		doSearch
功能:		基本的搜索功能的操作函数
参数:		readMsg:, serial:序列号
返回值:		int:搜索结果
*/
/*****************************************************************/
static int doSearch(LinpopReadMsg *readMsg,void *serial){
    if(readMsg->serial == *(guint *)serial)
        return (int)readMsg;
    else
        return 0;
}

/*****************************************************************/
/*
函数名:		freeSendMsg
功能:		释放一个LinpopReadMsg类型所占用的内存空间
参数:		sendMsg:LinpopReadMsg *
返回值:		void
*/
/*****************************************************************/
void freeReadMsg(LinpopReadMsg *readMsg){
    g_free(readMsg->msg);
    g_free(readMsg);
}
#endif
