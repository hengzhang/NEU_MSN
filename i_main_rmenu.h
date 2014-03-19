/********************************************/
/*名称：i_main_rmenu.h
/*描述： 1、该文件定义了程序中用于创建右键菜单的函数
	2、定义了相应的回调函数
/*作者：
/*日期：
/********************************************/
#ifndef I_MAIN_RMENU_H
#define I_MAIN_RMENU_H
#include <gtk/gtk.h>
/**********************************************/
/*
函数名：	GtkWidget* getMenu_right(void);
参数:	void
描述：	创建鼠标右键菜单
返回值：	窗体控件指针
*/
/**********************************************/
GtkWidget* getRightMenu(void);
/**********************************************/
/*
函数名：	void look_item(GtkWidget *widget, gpointer entry);
参数:	widget：控件指针；entry：控件指针
描述：	回调函数-查看好友资料
返回值：	void
*/
/**********************************************/
void lookUserProfile(gchar *str);
/**********************************************/
/*
函数名：	void talk_item(GtkWidget *widget, gpointer entry);
参数:	widget：控件指针；entry：控件指针
描述：	回调函数-单人聊天
返回值：	void
*/
/**********************************************/
void singleTalk(gchar *str);
/**********************************************/
/*
函数名：	void grouptalk(GtkWidget *widget, gpointer entry);
参数:	widget：控件指针；entry：控件指针
描述：	回调函数-群组聊天
返回值：	void
*/
/**********************************************/
void groupTalk(gchar *str);
/**********************************************/
/*
函数名：	void right_response(gchar *string);
参数:	string：菜单选项字符串
描述：	根据选中的菜单选项调用响应的函数
返回值：	void
*/
/**********************************************/
void switchForRMenu(gchar *string);
/**********************************************/
/*
函数名：	static gint right_button_press (GtkWidget * wid, GdkEvent * event);
参数:	wid:指向菜单的指针；event：事件
描述：	点击右键弹出菜单
返回值： gint
*/
/**********************************************/
static gint responseToRMenu(GtkWidget * wid, GdkEvent * event);
/**********************************************/
/*
函数名：	void ga_add_popup_menu (GtkWidget * wid, GtkMenu * menu);
参数:	wid:要添加的控件；menu:要添加的菜单
描述：	为控件添加菜单
返回值：	void
*/
/**********************************************/
void addMenuToPopup(GtkWidget * wid, GtkMenu * menu);
//void createProfileWindow(gchar *ip);
//****************************************************/
/*
函数名:	    move_group
参数：		widget : 事件调用的控件,entry ： 事件 调用的传入参数
返回值:	   void
功能：	     将用户移动 至其他组的回调函数
*/
void move_group(gchar *string);
//****************************************************/
/*
函数名:	    move_black
参数：		widget : 事件调用的控件,entry ： 事件 调用的传入参数
返回值:	   void
功能：	     将用户移动 至黑名单的回调函数
*/
void move_black(gchar *string);

#endif
