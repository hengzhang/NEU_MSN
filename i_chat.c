/********************************************/
/*名称：i_chat.c
/*描述：此文件定义了文件传输相关的函数
/*作者：
/*日期：
/********************************************/


#ifndef __I_CHAT_
#define __I_CHAT_
#include "i_chat.h"

#define NAME_LEN 256
#define MSG_BUF 8096
static DB *chatList;
typedef struct foreachParm_T
{
    chatFunc func;
    void *parm;
}foreachParm;

static int doForeach(DB_DAT *chatList,void *parm);
static int doSearch(LinpopChat *chat,void *addr);
static int searchDB_DAT(DB_DAT *chatData,void *parm);
/*static int printUser(DB_DAT *chatData,void *para);*/

/**************************************************/
/*函数名： initChatList
/*功能描述：初始化窗口列表
/*时间：
/*返回值：void
/*作者:
/***************************************************/
void initChatList(void)
{
    chatList = db_new(NULL);
}

/**************************************************/
/*函数名： getChatNumber
/*功能描述：获取窗口号码
/*时间：
/*返回值：void
/*作者:
/***************************************************/
unsigned int getChatNumber()
{
    return chatList->num;
}

/**************************************************/
/*函数名： deleteChatByIP
/*功能描述：删除窗口
/*时间：
/*参数：
	struct sockaddr_in addr：地址
/*返回值：void
/*作者:
/***************************************************/
gboolean deleteChatByIP(struct sockaddr_in addr)
{
    DB_DAT *chatData = NULL;
    if((chatData= (DB_DAT*)db_foreach(chatList,searchDB_DAT,&addr)) != 0)
    {
        LinpopChat *chat = (LinpopChat *)chatData->data;
	g_free(chat->msg);
        g_free(chat);
        dbd_free(chatList,chatData);
        return TRUE;
    }
    return FALSE;
}

/**************************************************/
/*函数名：addChat
/*功能描述：添加窗口
/*时间：
/*参数：
	LinpopChat *win：聊天窗口
/*返回值：void
/*作者:
/***************************************************/
void addChat(LinpopChat *win)
{
    LinpopChat *chat;
    foreachParm parm = {(chatFunc)doSearch,(void *)&win->from};
    //DB_DAT *chatData;
    if((chat = db_foreach(chatList,doForeach,&parm)) != 0)
        return ;
    chat = g_malloc(sizeof(*chat));
    chat->mainWindow = win->mainWindow;
    chat->showTextView  = win->showTextView;
    chat->inputTextView = win->inputTextView;
    chat->showWindow = win->showWindow;
    chat->inputWindow = win->inputWindow;
    chat->selfImage = win->selfImage;
    chat->otherImage = win->otherImage;
    chat->selfLabel = win->selfLabel;
    chat->otherLabel = win->otherLabel;
    chat->from = win->from;
    chat->msg = g_malloc(MSG_BUF);
    chat->msg[0] = '\0';
	dbd_append(chatList, chat);
    return ;

}

/**************************************************/
/*函数名：getChatByIP
/*功能描述：通过ip获取聊天窗口
/*时间：
/*参数：
	struct sockaddr_in addr：
        LinpopChat *chat：
/*返回值：gboolean
/*作者:
/***************************************************/
gboolean getChatByIP(struct sockaddr_in addr,LinpopChat *chat)
{
    LinpopChat *temp = NULL;
    foreachParm parm = {(chatFunc)doSearch,&addr};
    if((temp = db_foreach(chatList,doForeach,&parm)) != 0){
        chat->mainWindow = temp->mainWindow;
        chat->showTextView  = temp->showTextView;
        chat->inputTextView = temp->inputTextView;
        chat->showWindow = temp->showWindow;
        chat->inputWindow = temp->inputWindow;
        chat->selfImage = temp->selfImage;
        chat->otherImage = temp->otherImage;
        chat->selfLabel = temp->selfLabel;
        chat->otherLabel = temp->otherLabel;
        chat->from = temp->from;
	chat->msg = temp->msg;
        return TRUE;
    }
    else
        return FALSE;
}

/**************************************************/
/*函数名：chatListForeach
/*功能描述：
/*时间：
/*参数：
	chatFunc func：函数
        void *arg：
/*返回值：int
/*作者:
/***************************************************/
int chatListForeach(chatFunc func,void *arg)
{
    foreachParm parm = {func,arg};
    return db_foreach(chatList,doForeach,&parm);
}

/**************************************************/
/*函数名：doSearch
/*功能描述：
/*时间：
/*参数：
	LinpopChat *chat
        void *addr
/*返回值：int
/*作者:
/***************************************************/
static int doSearch(LinpopChat *chat,void *addr)
{
    struct sockaddr_in searchAddr = *(struct sockaddr_in *)addr;
    if( memcmp(&chat->from.sin_addr,&searchAddr.sin_addr,4) != 0)
        return 0;
    return (int)chat;
}

/**************************************************/
/*函数名：doForeach
/*功能描述：
/*时间：
/*参数：
	DB_DAT *chatList:指针窗口列表
        void *parm
/*返回值：int
/*作者:
/***************************************************/
static int doForeach(DB_DAT *chatList,void *parm)
{
    LinpopChat *chat = (LinpopChat *)chatList->data;
    chatFunc func = ((foreachParm *)parm)->func;
    void *arg = ((foreachParm *)parm)->parm;
    return func(chat,arg);
}

/**************************************************/
/*函数名：searchDB_DAT
/*功能描述：
/*时间：
/*参数：
	DB_DAT *chatData
        void *parm
/*返回值：int
/*作者:
/***************************************************/
static int searchDB_DAT(DB_DAT *chatData,void *parm)
{
    struct sockaddr_in addr = *(struct sockaddr_in *)parm;
    LinpopChat *chat = (LinpopChat *)chatData->data;
    if(memcmp(&chat->from.sin_addr,&addr.sin_addr,4) != 0)
        return 0;
    else
        return (int)chatData;

}

#endif
