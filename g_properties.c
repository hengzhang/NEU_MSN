/************************************************************/
/*名称：g_properties.c
/* 描述：定义与配置相关的函数操作	
/*作者：张珩
/*日期：2011.6.28
/**********************************************************/

#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>
#include <arpa/inet.h>

#define PROPERTIES_C__

#include "g_properties.h"
#include "m_linpop.h"


struct OPTION_T option = {
	NULL,
	NULL,
	NULL,
	NULL,
	0,
	NULL,
};


/****************************************************/
/*
函数名:		load_properties
功能：		加载配置
参数：		char *path：配置文件的路径, struct OPTION_T *prop：指向配置结构体的指针
返回值:		void
*/
/**************************************************/
void load_properties(char *path, struct OPTION_T *prop )
{
	gboolean def;
	static struct {
		gchar	*label;
		gchar	*msg;
	} absence_msg[] = {
		{ ("Away"),		("I'm away, now.") },
		{ ("Lunch"),		("I am getting lunch.") },
		{ ("Conference"),	("conference") },
		{ ("Visitor"),	("I've got visitor.") },
		{ ("Gone"),		("i've get out now.") },
		{ ("Homecoming"),	("homecoming") },
		{ ("ramble"),		("Rambling Rambling Rambling") },
		{ ("hehe"),		("BowWow!!") },
	};
	gint		i;
	gchar		buf[4096];
    sprintf(buf, "main/nick=%s", option.user);
	
	//加载配置
	for (i = 0; i < IPMSG_ABSENCE_NUM; i++) {
		sprintf(buf, "absence/%u=%s", i, absence_msg[i].label);
		sprintf(buf, "absence/%u_=%s", i, absence_msg[i].msg);

	}

	for (i = 0; i < SHORTCUT_USER_NUM; i++) {
		sprintf(buf, "shortcut_user/%u=", i);
		
	}
	
}

/****************************************************/
/*
函数名:		save_properties
功能：		保存配置
参数：		char *path：配置文件路径, struct OPTION_T *prop：保存配置的全局变量
返回值:		void
*/
/**************************************************/
void save_properties(char *path, struct OPTION_T *prop )
{
	gint		i;
	gchar		buf[4096];

	//保存配置
	for (i = 0; i < IPMSG_ABSENCE_NUM; i++) {
		sprintf(buf, "absence/%u", i);
		sprintf(buf, "absence/%u_", i);
	}

	for (i = 0; i < SHORTCUT_USER_NUM; i++) {
		sprintf(buf, "shortcut_user/%u", i);
	}
}


