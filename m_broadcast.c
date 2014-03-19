/************************************************************/
/*
	1、定义了与广播地址有关的函数操作
		a.广播地址遍历时使用的函数重定义——(*broadcastFunc)
		b.初始化广播信息——initBroadcastList
		c.设置广播地址——setBroadcasts
		d.遍历广播地址链表——broadcastListForeach
		e.底层的遍历函数---doForeach
		f.底层的插入函数appendBroadcastList	

/*作者：张珩
/*日期：2011.6.28
/**********************************************************/

#ifndef __M_BROADCAST_
#define __M_BROADCAST_
#include <netinet/in.h>
#include "m_broadcast.h"
#define BROADCASTLEN 256
static DB *broadcastList = NULL;
static void appendBroadcastList(char *p);
static int doForeach(DB_DAT *broadcastData,void *arg);


/******************************************************/
/*	定义与广播变量相关的结构体broadcastParm_T：
	func：进行遍历操作的函数
	parm：函数的参数
/******************************************************/
typedef struct broadcastParm_T{
    broadcastFunc func;
    void *parm;
}broadcastParm;



/****************************************************/
/*
函数名:		initBroadcastList
功能：		初始化广播列表
参数：		void
返回值:		void
*/
/**************************************************/
void initBroadcastList(void)
{
    broadcastList = db_new(NULL);
}


/****************************************************/
/*
函数名:		setBroadcastList
功能：		设置广播列表
参数：		char *lbcast：广播IP地址，类似255.255.255.255, char *bcasts：单个的广播IP地址列表
返回值:		void
*/
/**************************************************/
void setBroadcasts(char *lbcast,char *bcasts)
{
    gchar *p;
    gchar **parray,**q;
    db_delete(broadcastList);
    appendBroadcastList(lbcast);
    q = parray = g_strsplit(bcasts,",",BROADCASTLEN);
    //将ip加入到广播列表中
    for(p = *q;p;p = *(++q))
        appendBroadcastList(p);
    g_strfreev(parray);
}


/****************************************************/
/*
函数名:		broadcastListForeach
功能：		遍历广播地址链表
参数：		broadcastFunc func：进行遍历操作的函数, void *parm：函数的参数
返回值:		void
*/
/**************************************************/
int broadcastListForeach(broadcastFunc func,void *arg){
     broadcastParm parm= {func,arg};
     return db_foreach(broadcastList,doForeach,&parm);
}


/****************************************************/
/*
函数名:		doForeach
功能：		基本的遍历操作函数
参数：		DB_DAT *broadcastData：链表数据项, void *parm：遍历函数的参数（broadcastPram类型）
返回值:		static int
*/
/**************************************************/
static int doForeach(DB_DAT *broadcastData,void *parm){
     struct in_addr *addr = (struct in_addr *)broadcastData->data;
     broadcastFunc func = ((broadcastParm *)parm)->func;
     void *arg = ((broadcastParm *)parm)->parm;
     return func(addr,arg);
}


/****************************************************/
/*
函数名:		appendBroadcastList
功能：		插入广播地址
参数：		char *p：字符串形式的ip地址
返回值:		static void
*/
/**************************************************/
static void appendBroadcastList(char *p)
{
    struct in_addr *baddress;
    baddress = g_malloc(sizeof(baddress));
    //如果ip地址不合法，则提示错误信息
    //否则将ip地址加入到广播地址中
    if(inet_aton(p,baddress) == 0){
        if(*p != '\0')
            g_print("cannot convert %s to addressn",p);
    }
    else{
        dbd_append(broadcastList,baddress);
    }
}
#endif
