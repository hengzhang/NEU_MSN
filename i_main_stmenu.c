/********************************************/
/*名称：i_main_stmenu.c
/*描述： 1、该文件定义了程序中用于创建在线状态菜单的函数
	2、定义了相应的回调函数
/*作者：
/*日期：
/********************************************/
#ifndef __I_MAIN_STMENU_
#define __I_MAIN_STMENU_
#include "i_main_stmenu.h"
#include "i_main_window.h"
/*****************************************/
//由在线状态生成图标路径
/*****************************************/
static void responseStateChange(gchar *string);
static char* getPath(char* p);

/*******************************************/
/*
函数名:		static char* getPath(char* p)
参数:		p:所选择的状态选项字符串
函数描述:	将选定的状态选项转化为相应的状态图标文件名称
返回值:         	图片文件名称字符串
*/
/********************************************/

static char* getPath(char* p)
{
	int i;
	char* a[5] = {"我在线上","请勿打扰","离开","忙碌","隐身"};
	char* b[5] = {"Icon/available.svg","Icon/away.svg","Icon/busy.svg","Icon/idle.svg","Icon/invisible.svg"};
	for(i=0; i<5; i++)
		if(strcmp(p,a[i])==0)
			break;
	return b[i];
}
/*******************************************/
/*
函数名:		gboolean state_press( GtkWidget *widget,GdkEvent *event );
参数:		widget:按钮控件指针；event：事件指针
函数描述:	点击更换状态按钮时弹出菜单选项
返回值:         	布尔值
*/
/********************************************/
gboolean doStateButtonPress( GtkWidget *widget,GdkEvent *event )
{
	if (event->type == GDK_BUTTON_PRESS) {
		GdkEventButton *bevent = (GdkEventButton *) event;
		gtk_menu_popup (GTK_MENU(widget), NULL, NULL, NULL, NULL,bevent->button, bevent->time);
		return TRUE;
	}
	return FALSE;
}
/*******************************************/
/*
函数名:		static void responseStateChange(gchar *string)
参数:		string:图标文件名称
函数描述:	根据图片名称更改状态
返回值:         	空
*/
/********************************************/
static void responseStateChange(gchar *string)
{
	GtkWidget *image;
	image = gtk_image_new_from_file (getPath(string));
	gtk_button_set_image(GTK_BUTTON(online_state),image);
	gtk_label_set_text(GTK_LABEL(online),string);
	printf ("%s\n",string);
}
/*******************************************/
/*
函数名:		GtkWidget* getMenu_state(void);
参数:		空
函数描述:	创建在线状态选项菜单
返回值:         	控件指针
*/
/********************************************/
GtkWidget* getStateMenu(void){
	GtkWidget *menu_state,*menu_items_state;
	int i;
	char buf[128];
	menu_state = gtk_menu_new ();
	sprintf (buf, "我在线上");
	for (i = 0; i < 5; i++)
	{
	switch(i){
		case 0:
			sprintf (buf, "我在线上");
			break;
		case 1:
			sprintf (buf, "请勿打扰");
			break;
		case 2:
			sprintf (buf, "离开");
			break;
		case 3:
			sprintf (buf, "忙碌");
			break;
		case 4:
			sprintf (buf, "隐身");
			break;
	}
	menu_items_state = gtk_menu_item_new_with_label (buf);
	gtk_menu_shell_append (GTK_MENU_SHELL (menu_state), menu_items_state);
	g_signal_connect_swapped (G_OBJECT (menu_items_state), "activate",G_CALLBACK (responseStateChange),(gpointer) g_strdup(buf));
	gtk_widget_show (menu_items_state);
	}
	return menu_state;
}
#endif
