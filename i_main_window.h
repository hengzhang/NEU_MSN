/********************************************/
/*名称：i_main_window.h
/*描述： 1、该文件定义了程序中用于创建主界面的函数
	2、定义了程序中用到的部分全局变量
/*作者：
/*日期：
/********************************************/
#ifndef I_MAIN_WINDOW_H
#define I_MAIN_WINDOW_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>

/*
typedef struct _ip_name_trayIcon_msg
{
char *ip;
char *name;
GtkStatusIcon *trayIcon;
char *msg;
}tray_icon;
*/

//char * filename = "../Icon/bg2.jpg";

enum
{
  COLUMN = 0,
  NUM_COLS
} ;
/********************************************/
/*
函数名:		void creat_main(int argc,char *argv[]);
参数:		argc:参数个数；argv：字符数组
函数描述:	创建程序运行的主窗口
返回值:         	空
*/
/********************************************/
void createMainWindow(void);
/****************************************************/
/*
函数名:		remove_all
参数：		void
返回值:		void
功能：	    移除所有树形节点
*/
/**************************************************/
void remove_all(void);

//flag
#ifndef __I_MAIN_WINDOW_
extern GtkWidget *online;		//显示在线状态的文字信息
extern GtkWidget *online_state;	//显示在线状态的图标信息
extern GtkWidget *photo;		//用户头像
extern GtkWidget *name;		//用户名
extern GtkWidget *usr_state;		//心情
extern GtkWidget *window;
extern GtkWidget *vbox_mid;
extern GtkWidget *vbox_main;
extern GtkStatusIcon *trayIcon;
extern GtkWidget *entry;//中间过度控件，用于获取选项的值
extern GtkWidget *view;//分组显示好友列表的控件
extern gchar *selectText;
#endif
#endif
