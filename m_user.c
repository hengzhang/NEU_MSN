/********************************************/
/*名称：m_user.c
/*描述： 1、定义与用户信息有关的结构体
	2、______

/*作者：______
/*日期：______
/********************************************/
#ifndef __M_USER_
#define __M_USER_
#include "m_user.h"
static DB *userList;

/*******************************************************************/
/*
函数名:		initUserList
功能:		初始化用户列表
参数:		void
返回值:          void	
*/
/*****************************************************************/
void initUserList(void){
    userList = db_new(NULL);
}
typedef struct foreachParm_T{
    userFunc func;
    void *parm;
}foreachParm;
static int doForeach(DB_DAT *userList,void *parm);
static int doSearch(LinpopUser *user,void *addr);
static int searchDB_DAT(DB_DAT *userData,void *parm);

/*******************************************************************/
/*
函数名:		getUserNumber
功能:		获取用户总数
参数:		void
返回值:          int:用户总数
*/
/*****************************************************************/
unsigned int getUserNumber(){
    return userList->num;
}

/*****************************************************************/
/*
函数名:		deleteUserByIP
功能:		根据IP号删除用户
参数:		addr:______
返回值:		gboolean：TRUE——删除成功； FALSE——删除失败
*/
/*****************************************************************/
gboolean deleteUserByIP(struct sockaddr_in addr)
{
    DB_DAT *userData = NULL;
    //遍历用户列表，根据ip查找用户，若查找成功，则将其删除
    //否则返回false
    if((userData= (DB_DAT*)db_foreach(userList,searchDB_DAT,&addr)) != 0)
    {
        freeUser(userData->data);
        dbd_free(userList,userData);
        return TRUE;
    }
    return FALSE;
}

/*****************************************************************/
/*
函数名:		addUser
功能:		根据通过参数列表传进来的用户信息添加用户
参数:		userName:用户名, machine:______, nick:昵称, group:用户组, addr______:
返回值:		void
*/
/*****************************************************************/
void addUser(gchar *userName,gchar *machine,gchar *nick,gchar *group,struct sockaddr_in addr)
{
    LinpopUser *user;
    foreachParm parm = {(userFunc)doSearch,(void *)&addr};
    //DB_DAT *userData;
    //如果用户已经在列表中，则返回
    if((user = db_foreach(userList,doForeach,&parm)) != 0)
        return ;
    //初始化用户信息
	user= g_malloc(sizeof(LinpopUser));
    user->user = g_strdup(userName);
    user->machine = g_strdup(machine);
    user->nick = g_strdup(nick);
    user->group = g_strdup(group);
    user->from = addr;
	user->send = 0;
	user->recv = 0;
	user->dialup = FALSE;
	//userData = dbd_append(userList, user);
	//将用户添加到用户列表中
	dbd_append(userList, user);
    return ;

}

/*****************************************************************/
/*
函数名:		getUserByIP
功能:		根据IP号获得用户，并记录在user指针中
参数:		addr:______, user:______
返回值:		gboolean：TRUE——获取成功； FALSE——获取失败
*/
/*****************************************************************/
gboolean getUserByIP(struct sockaddr_in addr,LinpopUser *user){
    LinpopUser *temp = NULL;
    //根据ip在用户列表中查找用户
    foreachParm parm = {(userFunc)doSearch,&addr};
    if((temp = db_foreach(userList,doForeach,&parm)) != 0){
        user->nick = g_strdup(temp->nick);
        user->machine = g_strdup(temp->machine);
        user->user = g_strdup(temp->user);
        user->group = g_strdup(temp->group);
        user->recv = temp->recv;
        user->send = temp->send;
        user->dialup = temp->dialup;
        user->from = temp->from;
        return TRUE;
    }
    else
        return FALSE;
}
/*****************************************************************/
/*
函数名:		setNick
功能:		设置昵称
参数:		addr:______, nick:昵称
返回值:		void
*/
/*****************************************************************/
void setNick(struct sockaddr_in addr,gchar *nick){
    LinpopUser *temp = NULL;
    //根据用户的addr查找用户
    foreachParm parm = {(userFunc)doSearch,&addr};
    if((temp = (LinpopUser *)db_foreach(userList,doForeach,&parm)) != 0)
        temp->nick = g_strdup(nick);
}

/*****************************************************************/
/*
函数名:		setRecv
功能:		______
参数:		addr:______, send:______
返回值:		void
*/
/*****************************************************************/
void setRecv(struct sockaddr_in addr,guint recv){
    LinpopUser *temp = NULL;
    //根据用户的addr查找用户
    foreachParm parm = {(userFunc)doSearch,&addr};
    if((temp = (LinpopUser *)db_foreach(userList,doForeach,&parm)) != 0)
        temp->recv = recv;
}

/*****************************************************************/
/*
函数名:		setSend
功能:		______
参数:		addr:______, send:______
返回值:		void
*/
/*****************************************************************/
void setSend(struct sockaddr_in addr,guint send){
    LinpopUser *temp = NULL;
    //根据用户的addr查找用户
    foreachParm parm = {(userFunc)doSearch,&addr};
    if((temp = (LinpopUser *)db_foreach(userList,doForeach,&parm)) != 0)
        temp->send= send;
}

/*****************************************************************/
/*
函数名:		setDialup
功能:		______
参数:		addr:______, dialup:______
返回值:		void
*/
/*****************************************************************/
void setDialup(struct sockaddr_in addr,gboolean dialup){
    LinpopUser *temp = NULL;
    //根据用户的addr查找用户
    foreachParm parm = {(userFunc)doSearch,&addr};
    if((temp = (LinpopUser *)db_foreach(userList,doForeach,&parm)) != 0)
        temp->dialup = dialup;
}

/*****************************************************************/
/*
函数名:		freeUser
功能:		______
参数:		user:______
返回值:		void
*/
/*****************************************************************/
void freeUser(LinpopUser *user){
    g_free(user->nick);
    g_free(user->machine);
    g_free(user->user);
    g_free(user->group);
    g_free(user);
}

/*****************************************************************/
/*
函数名:		userListForeach
功能:		______
参数:		func:______, parm:______
返回值:		int:______
*/
/*****************************************************************/
int userListForeach(userFunc func,void *arg){
    foreachParm parm = {func,arg};
    return db_foreach(userList,doForeach,&parm);
}

/*****************************************************************/
/*
函数名:		doSearch
功能:		______
参数:		user:______, addr:______
返回值:		int:______
*/
/*****************************************************************/
static int doSearch(LinpopUser *user,void *addr)
{
    struct sockaddr_in searchAddr = *(struct sockaddr_in *)addr;
    if( memcmp(&user->from.sin_addr,&searchAddr.sin_addr,4) != 0)
        return 0;
    return (int)user;
}

/*****************************************************************/
/*
函数名:		doForeach
功能:		______
参数:		userList:用户列表, parm:______
返回值:		int:______
*/
/*****************************************************************/
static int doForeach(DB_DAT *userList,void *parm){
    LinpopUser *user = (LinpopUser *)userList->data;
    userFunc func = ((foreachParm *)parm)->func;
    void *arg = ((foreachParm *)parm)->parm;
    return func(user,arg);
}

/*****************************************************************/
/*
函数名:		searchDB_DAT
功能:		______
参数:		userData:______, parm:______
返回值:		int:______
*/
/*****************************************************************/
static int searchDB_DAT(DB_DAT *userData,void *parm){
    struct sockaddr_in addr = *(struct sockaddr_in *)parm;
    LinpopUser *user = (LinpopUser *)userData->data;
    if(memcmp(&user->from.sin_addr,&addr.sin_addr,4) != 0)
        return 0;
    else
        return (int)userData;

}

#endif
