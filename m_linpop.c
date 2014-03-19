/********************************************/
/*名称：m_linpop.c
/*描述： 1、定义了进行网络通信的函数
	2、定义了飞鸽命令的宏定义

/*作者：张珩
/*日期：2011.6.28
/********************************************/
#define M_LINPOP_C__

#include <gtk/gtk.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include "g_properties.h"
#include "m_user.h"
#include "m_readMsg.h"
#include "m_sendMsg.h"
#include "m_broadcast.h"
#include "m_linpop.h"
#include "m_kanji.h"
#include "g_db.h"

#define BUF_SIZE 2048
#define	DEFAULT_LINPOP_PORT	(2425)
#define	LINPOP_MSG_TOKEN		"\a"

static gchar	buffer[MAX_SOCKBUF*2];
static gchar	t_buff[MAX_SOCKBUF];

#define HOSTNAME_LENGTH 256
 
static gchar	hostname[HOSTNAME_LENGTH];
static guint16 	ipmsg_port = DEFAULT_LINPOP_PORT;
static struct sockaddr_in	me;
static struct sockaddr_in	from;
static gint fromlen;
static gint len;
static gint soc;			/* ¥œ¥±¥Ã¥È */
static fd_set soc_mask;		/* mask */
static gint soc_width;		/* width of the mask */


static gint interrupt_tag;

static gint nLinpopStatusMode = 0;
gint nLinpopNonPopup = 0;
static gint nLinpopNonPopupTimeoutTag;
static gint nLinpopNonPopupFlag = 0;


#define	iskanji(c)	(((c)>=0x81 && (c)<=0x9F) || ((c)>=0xE0 && (c)<=0xFC))
#define	iskanji2(c)	((c)>=0x40 && (c)<=0xFC && (c)!=0x7F)

/*******************************************************************/
/*
函数名:		stricmp
功能:		比较两个字符串，用于ASCIIZ
参数:		s:字符串， d:字符串
返回值:          int:-1——按字母排序s在d之前； 1——按字母排序s在d之前； 0——s与d相等	
*/
/*****************************************************************/
int stricmp(char* s, char* d)
{
		//对字符串逐个进行比较，一旦比较出大小，立即返回
		//如果字符串到达尾部仍相同，则比较字符串长度
		//若还未比较出大小，则表示两个字符串相同
    while(*s){
		if (toupper(*s) < toupper(*d))		return -1;
		else if (toupper(*s) > toupper(*d))	return 1;
		s++;
		d++;
    }
    if (strlen(s) < strlen(d))	return 1;
    /* equal */
    return 0;
}


/*******************************************************************/
/*
函数名:		stricmp
功能:		比较两个字符串的前n位，用于ASCIIZ
参数:		s:字符串， d:字符串， n:比较的位数
返回值:          int:-1——按字母排序s在d之前； 1——按字母排序s在d之前； 0——s与d相等	
*/
/*****************************************************************/
int strincmp(char* s, char* d, int n)
{
		//逐个比较字符串的前n位字符，一旦比较出大小，立即返回
		//若前n位大小相同，则比较字符串长度
		//若字符长度仍一样，则表示两字符串大小相同
    while(*s){
		if (n <= 0)	return 0;
		if (toupper(*s) < toupper(*d))		return -1;
		else if (toupper(*s) > toupper(*d))	return 1;
		n--;
		s++;
		d++;
    }
    if (strlen(s) < strlen(d))	return 1;
    /* equal */
    return 0;
}

/*******************************************************************/
/*
函数名:		LinpopFree_msg
功能:		释放一个LinpopMessage所占用的内存
参数:		msg:待释放的LinpopMessage结构体
返回值:          void
*/
/*****************************************************************/
void LinpopFree_msg(struct LinpopMessage_T *msg)
{
	//释放msg的成员
	g_free(msg->serial);
	g_free(msg->user);
	g_free(msg->machine);
	g_free(msg->group);
	g_free(msg->appendix);
	//释放msg
	g_free(msg);
}

/*******************************************************************/
/*
函数名:		LinpopSetup
功能:		初始化通讯功能
参数:		port:端口号
返回值:          void	
*/
/*****************************************************************/
void LinpopSetup(guint16 port)
{
	// get hostname
	if (gethostname(hostname, HOSTNAME_LENGTH) == -1) {
		g_error(("cannot find hostname\n"));
	}
    if (strchr(hostname, '.') != NULL)	*strchr(hostname, '.') = '\0';
	// alloc database
    initUserList();
    initSendMsgList();
    initReadMsgList();
    initBroadcastList();
	// set IP Messenger port
	if (port != 0)	ipmsg_port = port;
}

/*******************************************************************/
/*
函数名:		LinpopQuit
功能:		退出程序，通知下线
参数:		void
返回值:          void	
*/
/*****************************************************************/
void LinpopQuit(void)
{
	//发出下线广播
	LinpopSendBroadcast(LINPOP_BR_EXIT, LinpopGetNickWithAbsence());
}

/*******************************************************************/
/*
函数名:		LinpopDBEachSendDialup
功能:		底层发送挂断命令的函数
参数:		user:用户结构, para:发生的内容
返回值:          int：表明发送的结果	
*/
/*****************************************************************/
static int LinpopDBEachSendDialup(LinpopUser *user, void *para) 
{
	struct {
		guint32 com ;
		gchar *data ;
	} *sendbroadcast_para = para;

	if(user->dialup) 
		LinpopSend(&user->from, sendbroadcast_para->com, sendbroadcast_para->data);
	return 0;
}

/*******************************************************************/
/*
函数名:		LinpopDBEachSendBroadcast
功能:		底层发送广播信息的函数
参数:		baddress:单个的广播地址, para:广播地址字符串
返回值:          int：表明发送的状态	
*/
/*****************************************************************/
static int LinpopDBEachSendBroadcast(struct in_addr *baddress, void *para) 
{
	struct sockaddr_in	bcast;
	struct {
		guint32 com ;
		gchar *data ;
	} *sendbroadcast_para = para;
  
  //初始化
	memset((gchar*)&bcast, 0, sizeof(bcast));
	bcast.sin_family = AF_INET;
	bcast.sin_port = htons(ipmsg_port);
	bcast.sin_addr = *baddress;
	//发送消息
	LinpopSend(&bcast, sendbroadcast_para->com, sendbroadcast_para->data);
	return 0;
}

/*******************************************************************/
/*
函数名:		LinpopSendBroadcast
功能:		用户登录后向网络中的用户发送广播，通知用户在线
参数:		com:命令字, data:发送的数据
返回值:          void	
*/
/*****************************************************************/
void LinpopSendBroadcast(guint32 com, gchar *data)
{
	struct {
		guint32 com;
		gchar *data;
	} sendbroadcast_para;
 
	sendbroadcast_para.com = com;
	sendbroadcast_para.data = data;
	//给所有用户发送上线消息
	//db_foreach(db_broadcast, LinpopDBEachSendBroadcast, &sendbroadcast_para);
    broadcastListForeach(LinpopDBEachSendBroadcast,&sendbroadcast_para);
	//db_foreach(userList, LinpopDBEachSendDialup, &sendbroadcast_para);
    userListForeach(LinpopDBEachSendDialup,&sendbroadcast_para);
}

/*******************************************************************/
/*
函数名:		LinpopLogin
功能:		登录Linpop
参数:		void
返回值:          void	
*/
/*****************************************************************/
void LinpopLogin(void)
{
	int optval = 1;

	if (option.localnet_bcast == NULL)	option.localnet_bcast = "255.255.255.255";
    option.broadcasts = "10.10.31.5";
    setBroadcasts(option.localnet_bcast,option.broadcasts);
  //建立socket，初始化
	if ((soc = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		g_error(("socket alloc failed.\n"));
	}
	memset((gchar*)&me, 0, sizeof(me));
	me.sin_addr.s_addr = INADDR_ANY;
	me.sin_family = AF_INET;
	me.sin_port = htons(ipmsg_port);
	setsockopt(soc, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval));	
	if (bind(soc, &me, sizeof(me)) == -1) {
		g_error(("could not bind.\n"));
	}
	LinpopSendBroadcast(LINPOP_BR_ENTRY, LinpopGetNickWithAbsence());

	interrupt_tag = gdk_input_add(soc, GDK_INPUT_READ, LinpopMain, NULL);

	soc_width = soc+1;
	FD_ZERO(&soc_mask);
	FD_SET(soc, &soc_mask);
}


static const char *date_week[7] = {
	("Sun"), ("Mon"), ("Tue"), ("Wed"), ("Thu"), ("Fri"), ("Sat"),
};

static const char *date_month[12] = {
	("Jan"), ("Feb"), ("Mar"), ("Apr"), ("May"), ("Jun"), ("Jul"), ("Aug"), ("Sep"), ("Oct"), ("Nov"), ("Dec"),
};

/*******************************************************************/
/*
函数名:		LinpopGotMsg
功能:		解析发送的文本消息
参数:		msg:接受到的LINPOP_SENDMSG类型消息
返回值:          void	
*/
/*****************************************************************/
static void LinpopGotMsg(struct LinpopMessage_T *msg)
{
	time_t now = time(NULL);
	struct tm lc = *localtime(&now);
#define	INSERT_BUFF_SIZE	512
	gint		text_len;
	gchar		*pStr, *pp;
	LinpopReadMsg	*readmsg;
	FILE		*fp;
	gint		i;
    gboolean result;
    guint serial;
	GtkWidget *urlWidget;			/* for TEST */
	DB_DAT		*dbd;
    LinpopUser *user = g_malloc(sizeof(*user));
    result = getUserByIP(msg->from,user);
    if(result == TRUE){
        setRecv(user->from,user->recv + 1);
        freeUser(user);
    }else{
        g_free(user);
    }

	sprintf(t_buff, "%s[%s]\nfrom %s/%s\nat %s %s %2u %2u:%02u:%02u %4u", msg->user, "", msg->machine, inet_ntoa(from.sin_addr)
			, date_week[lc.tm_wday], date_month[lc.tm_mon], lc.tm_mday
			, lc.tm_hour, lc.tm_min, lc.tm_sec, lc.tm_year+1900);
	printf("%s[%s]\nfrom %s/%s\nat %s %s %2u %2u:%02u:%02u %4u", msg->user, "", msg->machine, inet_ntoa(from.sin_addr)
			, date_week[lc.tm_wday], date_month[lc.tm_mon], lc.tm_mday
			, lc.tm_hour, lc.tm_min, lc.tm_sec, lc.tm_year+1900);

	if ((msg->com2&LINPOP_BROADCASTOPT)) {
        g_print("Broadcast ..\n");
	}
	else if ((msg->com2&LINPOP_MULTICASTOPT)) {
	}

	text_len = strlen(msg->appendix);
    serial = atoi(msg->serial);
    addReadMsg(msg->appendix,serial,msg->from);

	for (i = 1, pStr = msg->appendix; *pStr; pStr++) {
		if (strincmp(pStr, "http://", 7) == 0 || strincmp(pStr, "ftp://", 6) == 0) {
			pp = t_buff;
			while (1) {
				if (*pStr == '\0' || !isgraph(*pStr)) {
					*pp = '\0';
                    setUrlFlag(serial,1);
					break;
				}
				*pp++ = *pStr++;
			}
		}
	}

	if ((msg->com2&LINPOP_SENDCHECKOPT)) {
		LinpopSend(&msg->from, LINPOP_RECVMSG, msg->serial);
	}
	/* if I am in absence mode, I'll send absence message */
	if (nLinpopStatusMode > 0 && !(msg->com2&LINPOP_AUTORETOPT)) {
        g_print("Fuzai OK\n");
		pStr = g_strdup(option.absence_text[nLinpopStatusMode-1]);
		LinpopSend(&msg->from, (LINPOP_SENDMSG|LINPOP_AUTORETOPT), stretos(pStr, pStr));
		g_free(pStr);
	}

	if ((msg->com2&LINPOP_SECRETOPT)) {
          g_print("secret information\n");

	}
	if (option.non_popup) {
		if (nLinpopNonPopup == 0) {
			nLinpopNonPopupFlag = 0;
		}
		nLinpopNonPopup++;
	}
	else {
        setShown(serial,1);
		if ((msg->com2&LINPOP_SECRETOPT)) {
		}
		else {
		}
	}
	if (option.beep)	gdk_beep();
}

/*******************************************************************/
/*
函数名:		LinpopPrintMsg
功能:		打印消息信息
参数:		msg:消息
返回值:          void	
*/
/*****************************************************************/
void LinpopPrintMsg(LinpopMessage *msg)
{
	time_t now = time(NULL);
	struct tm lc = *localtime(&now);

	//打印消息
	if (option.check_seal) {
		printf( ("%s[%s] (%s) - gipmsg"), msg->user, "", msg->machine);
		printf( ("%s[%s] (%s)\nopened\n  (at %s %2u %2u:%02u)  ")
				, msg->user, "", msg->machine
				, date_month[lc.tm_mon], lc.tm_mday
				, lc.tm_hour, lc.tm_min);
	}
}

/*******************************************************************/
/*
函数名:		LinpopUserEntry
功能:		添加一个用户
参数:		msg:消息
返回值:          void	
*/
/*****************************************************************/
static void LinpopUserEntry(struct LinpopMessage_T *msg)
{
		//添加指定用户
    addUser(msg->user,msg->machine,msg->appendix,msg->group,msg->from);

}

/*******************************************************************/
/*
函数名:		LinpopUserExit
功能:		删除一个用户
参数:		msg:消息
返回值:          void	
*/
/*****************************************************************/
static void LinpopUserExit(struct LinpopMessage_T *msg)
{
	DB_DAT		*dbd;
    gboolean result;
    //删除用户
    result = deleteUserByIP(msg->from);

}

/*******************************************************************/
/*
函数名:		LinpopSwitch
功能:		根据消息类型进行相应的转发
参数:		msg:接收到的原始消息
返回值:          void	
*/
/*****************************************************************/
void LinpopSwitch(LinpopMessage *msg)
{
	LinpopUser	*data;
	DB_DAT		*dbd;
	LinpopSendMsg	*smsg;
	GtkCList*		clist;
    gboolean result;
    guint serial;

	switch (msg->com) {
	case LINPOP_NOOPERATION:		/* ÌµÁàºî */
		g_print("[ LINPOP_BR_NOOPERATION ]\n");
		break;
	case LINPOP_BR_ENTRY:		/* ¥µ¡Œ¥Ó¥¹€Ë¥š¥ó¥È¥ê¡Êµ¯Æ°»þ€ËBroadcast¡Ë */
	    g_print("[ LINPOP_BR_ENTRY ]\n");
		LinpopUserEntry(msg);
		if (nLinpopStatusMode > 0)	LinpopSend(&from, (LINPOP_ANSENTRY|LINPOP_ABSENCEOPT), LinpopGetNickWithAbsence());
		else						LinpopSend(&from, LINPOP_ANSENTRY, LinpopGetNickWithAbsence());
		break;
	case LINPOP_BR_EXIT:			/* ¥µ¡Œ¥Ó¥¹€«€éÈŽ€±€ë¡ÊœªÎ»»þ€ËBroadcast¡Ë */
		g_print("[ LINPOP_BR_EXIT ]\n");
		LinpopUserExit(msg);
		break;
	case LINPOP_ANSENTRY:		/* ¥š¥ó¥È¥ê€òÇ§Œ±€·€¿€³€È€òÄÌÃÎ */
		g_print("[ LINPOP_ANSENTRY ]\n");
		LinpopUserEntry(msg);
		break;
	case LINPOP_BR_ABSENCE:		/* ÉÔºß¥â¡Œ¥ÉÊÑ¹¹ */
		g_print("[ LINPOP_BR_ABSENCE ]\n");
        setNick(msg->from,msg->appendix);

		break;
	case LINPOP_SENDMSG:			/* ¥á¥Ã¥»¡Œ¥ž€ÎÁ÷¿® */
		g_print("[ LINPOP_SENDMSG ]\n");
		LinpopGotMsg(msg);
		break;
	case LINPOP_RECVMSG:			/* ¥á¥Ã¥»¡Œ¥ž€ÎŒõ¿®³ÎÇ§ */
		g_print("[ LINPOP_RECVMSG ]\n");
        LinpopSendMsg *sendMsg = g_malloc(sizeof(*sendMsg));
        serial = atoi(msg->appendix);
        result = getSendMsgBySerial(serial,sendMsg);
        if(result == TRUE){
            g_print("delete message\n");
            gtk_timeout_remove(sendMsg->timeout_tag);
            deleteSendMsgBySerial(serial);
            freeSendMsg(sendMsg);
        }
        
		else {
			g_warning(("LINPOP_RECVMSG: unknown message"));
            g_free(sendMsg);
		}
		break;
	case LINPOP_READMSG:			/* Éõœñ€Î³«ÉõÄÌÃÎ */
		g_print("[ LINPOP_READMSG ]\n");
		LinpopPrintMsg(msg);
		break;
	}
	LinpopFree_msg(msg);
}

/*******************************************************************/
/*
函数名:		LinpopMain
功能:		进行消息接收的底层函数
参数:		data:传递的参数, source:未使用, condition:未使用
返回值:          void	
*/
/*****************************************************************/
void LinpopMain(gpointer data, gint source, GdkInputCondition condition)
{
	fd_set	readOk;
	struct timeval tv;
	LinpopMessage	*msg;
	gchar	*pchr;
	gint	nSep;
    gchar *ver;

	gdk_input_remove(interrupt_tag);

	tv.tv_sec = tv.tv_usec = 0;
	while (1) {
		readOk = soc_mask;
        g_print("while OK\n");
		if (!select(soc_width, (fd_set*)&readOk, NULL, NULL, &tv))	break;
		if (FD_ISSET(soc, &readOk)) { /* ¥œ¥±¥Ã¥È€Ë¥Ç¡Œ¥¿€¬€¢€ë€« */
			fromlen = sizeof(from);
			if ((len = recvfrom(soc, t_buff, MAX_SOCKBUF, 0, &from, &fromlen)) == -1)
				continue;
			pchr = t_buff;
			for (nSep = 0; len > 1; len--, pchr++) {
				if (nSep < 5) {
					if (*pchr == ':') {
						*pchr = '\a';
						nSep++;
					}
				}
				else {
					if (*pchr == '\0')	*pchr = '\a';
				}
			}
			strstoe(buffer, t_buff);
            ver = strtok(buffer,LINPOP_MSG_TOKEN);
			if ((strcmp(ver, LINPOP_MSG_TOKEN), "1") != 0) { /* version³ÎÇ§ */
                g_print("version is:%s",ver);
				g_warning(("illegal version of ipmsg protocol"));
			}
			msg = g_malloc(sizeof(*msg));
			msg->from = from;
			msg->from.sin_family = AF_INET;
			msg->from.sin_port = htons(ipmsg_port);
			msg->fromlen = fromlen;
			msg->serial = g_strdup(strtok(NULL, LINPOP_MSG_TOKEN));
			msg->user = g_strdup(strtok(NULL, LINPOP_MSG_TOKEN));
			msg->machine = g_strdup(strtok(NULL, LINPOP_MSG_TOKEN));
			msg->com = atoi(strtok(NULL, LINPOP_MSG_TOKEN));
			msg->com2 = msg->com&0xffffff00;
			msg->com &= 0xff;
			msg->appendix = g_strdup(strtok(NULL, LINPOP_MSG_TOKEN));
			if ((pchr = strtok(NULL, LINPOP_MSG_TOKEN)) != 0)	msg->group = g_strdup(pchr);
			else												msg->group = g_strdup("");
			LinpopSwitch(msg);	/* ³ÆœèÍý€Ø */
			LinpopResetTooltip();
		}
		else
			break;
	}
    g_print("OK\n");
	interrupt_tag = gdk_input_add(soc, GDK_INPUT_READ, LinpopMain, NULL);
}

/*******************************************************************/
/*
函数名:		LinpopReSendData
功能:		重新发送消息
参数:		dat:指向字符串的指针
返回值:          void	
*/
/*****************************************************************/
void gint LinpopReSendData(gpointer dat)
{
    gboolean result;
	LinpopSendMsg	*smsg = g_malloc(sizeof(*smsg));
	GtkWidget	*dialog;
    result = getSendMsgBySerial(*(guint *)dat,smsg);
    if(result == FALSE){
        g_free(smsg);
        return FALSE;
    }
    if((smsg->retry +1 ) >= LINPOP_RETRY_TIMES){
        gtk_timeout_remove(smsg->timeout_tag);
        freeSendMsg(smsg);
        return TRUE;
    }       
    setRetry(smsg->serial,smsg->retry + 1);

	LinpopSendSetSerial(&smsg->to, smsg->serial, smsg->com|LINPOP_RETRYOPT, smsg->msg);
    freeSendMsg(smsg);
	return TRUE;
}

/*******************************************************************/
/*
函数名:		LinpopSend
功能:		发送UDP数据的底层函数
参数:		dest:目的IP, com:命令字, data:发送的数据
返回值:          gint:发送的序列号	
*/
/*****************************************************************/
static gint LinpopSend(struct sockaddr_in *dest, guint32 com, gchar *data)
{
	time_t now = time(NULL);	/* packet no */
	static guint	last_time = 0;

	memset(buffer, 0, MAX_SOCKBUF);
	if ((guint)now <= last_time)	now = ++last_time;
	last_time = now;
	return LinpopSendSetSerial(dest, now, com, data);
}

/*******************************************************************/
/*
函数名:		LinpopSendSetSerial
功能:		设置了序列号的最底层函数
参数:		dest:目的IP, serial:指定的序列号, com:命令字, data:发送的数据
返回值:          gint:发送的序列号
*/
/*****************************************************************/
static gint LinpopSendSetSerial(struct sockaddr_in *dest, gint serial, guint32 com, gchar *data)
{
	gint		len, i;
	gchar		*p;

	memset(buffer, 0, MAX_SOCKBUF);
	sprintf(buffer, "1:%u:%s:%s:%u:%s", serial, option.user, hostname, com, data);
	len = strlen(buffer)+1;
	for (i = 0, p = buffer; i < len-1; i++, p++) {
		if (*p == '\a')	*p = '\0';
	}
	//发送消息
	sendto(soc, buffer, len, 0, dest, sizeof(struct sockaddr_in));
	return serial;
}


/*******************************************************************/
/*
函数名:		LinpopSendData
功能:		向单个用户发送聊天消息
参数:		addr:目的IP, content:消息内容, flag:附加命令字
返回值:          int:发送状态
*/
/*****************************************************************/
int LinpopSendData(struct sockaddr_in addr, const char *content, guint32 flag)
{
	time_t now = time(NULL);
	struct tm lc = *localtime(&now);
	FILE	*fp;
	gchar		str[BUF_SIZE];
    gboolean result;
    guint serial;
    guint *resendSerial = NULL;
    guint com;
    gint timeoutTag;
    LinpopUser *user = g_malloc(sizeof(*user));
    result = getUserByIP(addr,user);
    if(result == FALSE){
        g_print("Not this User\n");
        g_free(user);
        return -1;
    }
    setSend(user->from,user->send + 1);
	g_print("user:%s machine:%s\n", user->user, user->machine);
	if (option.log && (flag&LINPOP_NOLOGOPT) == 0) {
		if ((fp = fopen(option.log_fname, "a+")) != NULL) {
			printf( "%s[%s] (%s/%s)\nat %s %s %2u %2u:%02u:%02u %4u", user->user, "", user->machine, inet_ntoa(user->from.sin_addr)
					, date_week[lc.tm_wday], date_month[lc.tm_mon], lc.tm_mday
					, lc.tm_hour, lc.tm_min, lc.tm_sec, lc.tm_year+1900);

			fputs("================================\nTo: ", fp);
			fputs(t_buff, fp);
			if ((flag&LINPOP_SECRETOPT))	fputs(" (seal)", fp);
			fputs("\n--------------------------------\n", fp);
			fputs(str, fp);
			fputs("\n\n", fp);
			fclose(fp);
		}
	}
	stretos(str, content);
    com = (LINPOP_SENDMSG|LINPOP_SENDCHECKOPT)|flag;
    serial = LinpopSend(&user->from,com,str);
    addSendMsg(serial,user->user,user->machine,com,str,user->from);
    resendSerial = g_malloc(sizeof(*resendSerial));
    *resendSerial = serial;
    timeoutTag = gtk_timeout_add(LINPOP_TIMERINTERVAL,LinpopReSendData,resendSerial);
    setTimeoutTag(serial,timeoutTag);
    freeUser(user);
	
	return 0;
}

/*******************************************************************/
/*
函数名:		LinpopSendDataBroadcast
功能:		发送广播消息
参数:		content:消息内容, flag:命令字
返回值:          int:发送状态
*/
/*****************************************************************/
int LinpopSendDataBroadcast(const char *content, guint32 flag)
{
	gchar		str[BUF_SIZE];

	stretos(str, content);
	//发送广播
	LinpopSendBroadcast((LINPOP_SENDMSG|flag), str);
	return 0;
}

static void LinpopResetTooltip(void)
{
	gchar	buf[64];
    unsigned int num = getUserNumber();
	if (option.non_popup)
		printf( ("GIPMSG - port %u\n %u users logged on\n You've %d messages"), ipmsg_port, num, nLinpopNonPopup);
	else
		printf( ("GIPMSG - port %u\n %u users logged on"), ipmsg_port, num);
}



/*******************************************************************/
/*
函数名:		LinpopGetNickWithAbsence
功能:		获取昵称和心情短语
参数:		void
返回值:          gchar *:昵称‘\a’心情短语
*/
/*****************************************************************/
static gchar *LinpopGetNickWithAbsence(void)
{
	static gchar	buf[64];

	if (nLinpopStatusMode > 0)	sprintf( "%s[%s]%c%s", option.nick, option.absence[nLinpopStatusMode-1], '\a', option.group);
	else						sprintf( "%s%c%s", option.nick, '\a', option.group);
	stretos(buf, buf);
	return (char*)buf;
}

/*******************************************************************/
/*
函数名:		LinpopStatusChange
功能:		改变用户状态
参数:		mode:状态模式，值分别为：LINPOP_BR_ABSENCE,LINPOP_BR_ABSENCE|LINPOP_ABSENCEOPT
返回值:          void
*/
/*****************************************************************/
void LinpopStatusChange(gint mode)
{
	nLinpopStatusMode = mode;
	if (mode == 0) {
		LinpopSendBroadcast(LINPOP_BR_ABSENCE, LinpopGetNickWithAbsence());
        g_print("receive fuzaichange,mode is 0\n");
	}
	else {
		LinpopSendBroadcast((LINPOP_BR_ABSENCE|LINPOP_ABSENCEOPT), LinpopGetNickWithAbsence());
        g_print("receive fuzaichange,mode is 1\n");
	}
}



