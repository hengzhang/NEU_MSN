/********************************************/
/*名称：m_linpop.h
/*描述： 1、定义了进行网络通信的函数
	2、定义了飞鸽命令的宏定义

/*作者：张珩
/*日期：2011.6.28
/********************************************/
#ifndef M_LINPOP_H
#define M_LINPOP_H

#include <netinet/in.h>
#include <gtk/gtk.h>
#include "g_db.h"

/*  IP Messenger Communication Protocol version 1.0 define  */
/*  macro  */
#define GET_MODE(command)	(command & 0x000000ffUL)
#define GET_OPT(command)	(command & 0xffffff00UL)

/*  header  */
#define LINPOP_VERSION			0x0001
#define LINPOP_DEFAULT_PORT		0x0979

/*  command  */
#define LINPOP_NOOPERATION		0x00000000UL

#define LINPOP_BR_ENTRY			0x00000001UL
#define LINPOP_BR_EXIT			0x00000002UL
#define LINPOP_ANSENTRY			0x00000003UL
#define LINPOP_BR_ABSENCE		0x00000004UL

#define LINPOP_BR_ISGETLIST		0x00000010UL
#define LINPOP_OKGETLIST			0x00000011UL
#define LINPOP_GETLIST			0x00000012UL
#define LINPOP_ANSLIST			0x00000013UL
#define LINPOP_BR_ISGETLIST2		0x00000018UL

#define LINPOP_SENDMSG			0x00000020UL
#define LINPOP_RECVMSG			0x00000021UL
#define LINPOP_READMSG			0x00000030UL
#define LINPOP_DELMSG			0x00000031UL

#define LINPOP_GETINFO			0x00000040UL
#define LINPOP_SENDINFO			0x00000041UL

#define LINPOP_GETABSENCEINFO	0x00000050UL
#define LINPOP_SENDABSENCEINFO	0x00000051UL

/*  option for all command  */
#define LINPOP_ABSENCEOPT		0x00000100UL
#define LINPOP_SERVEROPT			0x00000200UL
#define LINPOP_DIALUPOPT			0x00010000UL

/*  option for send command  */
#define LINPOP_SENDCHECKOPT		0x00000100UL
#define LINPOP_SECRETOPT			0x00000200UL
#define LINPOP_BROADCASTOPT		0x00000400UL
#define LINPOP_MULTICASTOPT		0x00000800UL
#define LINPOP_NOPOPUPOPT		0x00001000UL
#define LINPOP_AUTORETOPT		0x00002000UL
#define LINPOP_RETRYOPT			0x00004000UL
#define LINPOP_PASSWORDOPT		0x00008000UL
#define LINPOP_NOLOGOPT			0x00020000UL
#define LINPOP_NEWMUTIOPT		0x00040000UL
#define LINPOP_NOADDLISTOPT		0x00080000UL

#define HOSTLIST_DELIMIT	"\a"
#define HOSTLIST_DUMMY		"\b"

/*  end of IP Messenger Communication Protocol version 1.0 define  */


/*  IP Messenger for Windows  internal define  */
#define LINPOP_REVERSEICON			0x0100
#define LINPOP_TIMERINTERVAL			4000
#define LINPOP_CLOCKTICK				500
#define LINPOP_GETLIST_FINISH		0

#define LINPOP_BROADCAST_TIMER		0x0101
#define LINPOP_SEND_TIMER			0x0102
#define LINPOP_DELETE_TIMER			0x0103
#define LINPOP_LISTGET_TIMER			0x0104
#define LINPOP_LISTGETRETRY_TIMER	0x0105
#define LINPOP_ENTRY_TIMER			0x0106
#define LINPOP_DUMMY_TIMER			0x0107

#define	LINPOP_RETRY_TIMES		4

#define LINPOP_NICKNAME			1
#define LINPOP_FULLNAME			2

#define LINPOP_NAMESORT			0x00000000
#define LINPOP_IPADDRSORT		0x00000001
#define LINPOP_HOSTSORT			0x00000002
#define LINPOP_NOGROUPSORTOPT	0x00000100
#define LINPOP_ICMPSORTOPT		0x00000200
#define LINPOP_NOKANJISORTOPT	0x00000400
#define LINPOP_ALLREVSORTOPT		0x00000800
#define LINPOP_GROUPREVSORTOPT	0x00001000
#define LINPOP_SUBREVSORTOPT		0x00002000

#if defined WIN32 || defined XWINDOW
#define MAX_SOCKBUF		32768
#else
#define MAX_SOCKBUF		8192
#endif
#define MAX_UDPBUF		8192
#define MAX_BUF			1024
#define MAX_NAMEBUF		50
#define MAX_LANGBUF		10
#define MAX_LISTBUF		(MAX_NAMEBUF * 3 + 50)

#define HS_TOOLS		"HSTools"
#define LINPOP			"Linpop"
#define NO_NAME			"no_name"
#define URL_STR			"://"
#define MAILTO_STR		"mailto:"


void LinpopSetup(guint16 port);
void LinpopLogin(void);
void LinpopQuit(void);
void LinpopSendBroadcast(guint32 com,gchar *data);
int LinpopSendData(struct sockaddr_in addr, const char *content, guint32 flag);
int LinpopSendDataBroadcast (const char *content, guint32 flag);
void LinpopStatusChange(gint mode);
gchar *LinpopGetNickWithAbsence(void);

#endif /* M_LINPOP_H */
