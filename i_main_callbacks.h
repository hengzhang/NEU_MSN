/********************************************/
/*名称：i_main_callbacks.h
/*描述： 1、该文件定义了程序中通用的功能函数
	2、定义了主界面的回调函数
/*作者：
/*日期：
/********************************************/
#ifndef I_MAIN_CALLBACKS_H
#define I_MAIN_CALLBACKS_H
#include <gtk/gtk.h>
/*******************************************************************/
/*
函数名:		void onChanged(GtkWidget *widget, gpointer entry);
参数:		widget: 控件指针,  entry : 回调函数调用参数
返回值:          空
函数描述:    改变树形菜单列表改变
*/
void onChanged(GtkWidget *widget, gpointer entry);
/*******************************************************************/
/*
函数名:		void startChat(GtkTreeView *treeView,GtkTreePath *path);
参数:		treeView: 传入的树形视图,  path : 得到树形的路径
返回值:          空
函数描述:    调用对话框
*/
void startChat(GtkTreeView *treeView,GtkTreePath *path);
/*******************************************************************/
/*
函数名:		void list_refresh(void);
参数:		空
函数描述:	刷新好友列表
返回值:          空
*/
void listRefresh(void);
/*******************************************************************/
/*
函数名:		void show_about(void);
参数:		空
函数描述:	软件帮助信息
返回值:          空
*/
void show_about(void);
/*******************************************************************/
/*
函数名:		void searchFriend( GtkWidget *widget,GtkWidget *entry );
参数:		widget ： 用户查找的控件 ，  entry ： 用户查找的IP 文本输入框
函数描述:    查找好友的回调函数
返回值:          空
*/
void searchFriend( GtkWidget *widget,GtkWidget *entry );
/*******************************************************************/
/*
函数名:		void clearEntry(GtkWidget *widget,GtkWidget *search);
参数:		widget ： 用户查找的控件 ，  entry ： 用户查找的IP 文本输入框
函数描述:    清空用户查找框
返回值:          空
*/
void clearEntry(GtkWidget *widget,GtkWidget *search);
/*******************************************************************/
/*
函数名:		void trayView(GtkMenuItem *item, gpointer window);
参数:		item:菜单项指针；window：程序主窗体指针
函数描述:	回调函数，显示主窗体
返回值:          空
*/
void trayView(GtkMenuItem *item, gpointer window);
/*******************************************************************/
/*
函数名:		void trayIconPopup(GtkStatusIcon *status_icon, guint button, guint32 activate_time, gpointer popUpMenu);
参数:		status_icon:任务栏图标指针；popUpMenu：弹出菜单指针
函数描述:	回调函数-右键单机任务栏图标，弹出选择菜单。
返回值:          空
*/
void trayIconPopup(GtkStatusIcon *status_icon, guint button, guint32 activate_time, gpointer popUpMenu);
/*******************************************************************/
/*
函数名:		void show_question(GtkWidget *widget, gpointer window);
参数:		widget:控件指针；window：主窗体指针
函数描述:	用户关闭程序时提示信息
返回值:          空
*/
void show_question(GtkWidget *widget, gpointer window);
/*******************************************************************/
/*
函数名:		void closeTimeoutWindow(GtkWidget *widget);
参数:		widget:控件指针；
函数描述:	用户关闭程序时提示信息
返回值:          空
*/
gboolean closeTimeoutWindow(GtkWidget *widget);
/*******************************************************************/
/*
函数名:		gboolean window_state_event (GtkWidget *widget, GdkEventWindowState *event, gpointer trayIcon);
返回值:          布尔值
*/
gboolean window_state_event (GtkWidget *widget, GdkEventWindowState *event, gpointer data);


/*******************************************************************/
/*
函数名:		gboolean window_state_event (GtkWidget *widget, GdkEventWindowState *event, gpointer trayIcon);
返回值:          布尔值
*/
void trayIconActivated(GObject *trayIcon, gpointer window);
#endif
