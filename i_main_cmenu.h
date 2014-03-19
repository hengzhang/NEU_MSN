/********************************************/
/*名称：get_vbox_mid.h
/*描述：1.该文件定义了用于创建皮肤选择菜单的函数
/*     2.定义了皮肤选择菜单的回调函数。
/*作者：
/*日期：
/********************************************/
#ifndef I_MAIN_CMENU_H
#define I_MAIN_CMENU_H
#include <gtk/gtk.h>
/*******************************************/
/*
函数名:		char* getStyle(char* p);
参数:		p:所选择的皮肤选项字符串
函数描述:	将选定的皮肤选项转化为相应的皮肤图片文件名称
返回值:         	皮肤图片文件名称字符串
*/
/********************************************/
//char* getStyle(char* p);
/*******************************************/
/*
函数名:		gboolean doStylePress( GtkWidget *widget,GdkEvent *event );
参数:		widget:按钮控件指针；event：事件指针
函数描述:	点击更换皮肤按钮时弹出菜单选项
返回值:         	布尔值
*/
/********************************************/
gboolean doStylePress( GtkWidget *widget,GdkEvent *event );
/*******************************************/
/*
函数名:		void responseToStyleChange(gchar *string);
参数:		string:图片文件名称
函数描述:	根据图片名称更改皮肤
返回值:         	空
*/
/********************************************/
//void responseToStyleChange(gchar *string);
/*******************************************/
/*
函数名:		GtkWidget* getMenu_style(void);
参数:		空
函数描述:	创建皮肤选项菜单
返回值:         	控件指针
*/
/********************************************/
GtkWidget* getStyleMenu(void);
//void show_warning(GtkWidget *window);

#endif
