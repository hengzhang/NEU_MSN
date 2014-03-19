/********************************************/
/*名称：g_properties.h
/*描述：与配置文件相关的结构体定义
/*作者：张珩
/*日期：2011.6.28
/********************************************/

#ifndef PROPERTIES_H__
#define PROPERTIES_H__
#define IPMSG_ABSENCE_NUM        8
#define	SHORTCUT_USER_NUM       10  
#include <stddef.h>
#include <gtk/gtk.h>


/******************************************************/
/*	定义配置的结构体OPTION_T：
	user：用户名
	nick：用户昵称
	group：用户所在组
	geometry：用户所属地
	port：通讯端口
	localnet_bcast：本地广播地址
	broadcasts：广播地址集合，格式：ip1,ip2,ip3,...,
	applet：是否任务栏图标显示
	default_sort_column：默认用户排序的比较项
	default_sort_type：默认的用户排序方法
	absence[IPMSG_ABSENCE_NUM]：用户状态，包括在线，隐身，离开等
	absence_text[IPMSG_ABSENCE_NUM]：心情短语
	shortcut_user[SHORTCUT_USER_NUM]：用户简介
	check_seal	:1 ：//未使用
	non_popup	:1 ：是否自动弹出窗口
	seal		:1 ：是否密送
	cite		:1 ：//未使用
	log		:1 ：是否进行日志记录
	log_fname：日志文件名
	beep		:1 ：是否进行声音提示
	snd		:1 ：//未使用
	snd_fname： //未使用
/******************************************************/
struct OPTION_T {
	char	*user;
	char	*nick;
	char	*group;
	char	*geometry;
	guint16	port;
	char	*localnet_bcast;
	char	*broadcasts;
	int		applet;
	int		default_sort_column;
	int		default_sort_type;
	char	*absence[IPMSG_ABSENCE_NUM];
	char	*absence_text[IPMSG_ABSENCE_NUM];
	char	*shortcut_user[SHORTCUT_USER_NUM];
	unsigned		check_seal		:1;
	unsigned		non_popup		:1;
	unsigned		seal			:1;
	unsigned		cite			:1;
	unsigned		log				:1;
	char	*log_fname;
	unsigned		beep			:1;
	unsigned		snd				:1;
	char	*snd_fname;
};


void load_properties(char *path, struct OPTION_T *prop );
void save_properties(char *path, struct OPTION_T *prop );



#ifndef PROPERTIES_C__
/****************************************************/
/*
外部变量：	struct OPTION_T option：全局的配置结构体变量
*/
/**************************************************/
extern struct OPTION_T option;
#endif /* PROPERTIES_C__ */


#endif /* PROPERTIES_H__ */
