/********************************************/
/*名称：i_main_cmenu.c
/*描述：1.该文件定义了用于创建皮肤选择菜单的函数
/*     2.定义了皮肤选择菜单的回调函数。
/*作者：
/*日期：
/********************************************/
#ifndef __I_MAIN_CMENU_
#define __I_MAIN_CMENU_
#include "i_main_cmenu.h"
#include "i_main_window.h"
/****************************************/
//由选择的皮肤转化为相应的图片文件路径
/****************************************/
static void responseToStyleChange(gchar *string);
static void showWarning(GtkWidget *window);
char* getStyle(char* p);
/*******************************************/
/*
函数名:		char* getstyle(char* p);
参数:		p:所选择的皮肤选项字符串
函数描述:	将选定的皮肤选项转化为相应的皮肤图片文件名称
返回值:         	皮肤图片文件名称字符串
*/
/********************************************/
char* getStyle(char* p)
{
	int i;
	char* a[5] = {"经典","复古","原始","自然","默认"};
	char* b[5] = {"Icon/bg1.jpg","Icon/bg2.jpg","Icon/bg3.jpg","Icon/bg4.jpg","Icon/bg5.jpg"};
	for(i=0; i<5; i++)
		if(strcmp(p,a[i])==0)
			break;
	return b[i];
}
static void showWarning(GtkWidget *window)
{
  GtkWidget *dialog;
  dialog = gtk_message_dialog_new(window,
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_WARNING,
            GTK_BUTTONS_OK,
            "您还不是linpop会员，注册linpop会员即可\n享受更换皮肤及五十余项会员特权！");
  gtk_window_set_title(GTK_WINDOW(dialog), "Warning");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}
/*******************************************/
/*
函数名:		gboolean doStylePress( GtkWidget *widget,GdkEvent *event );
参数:		widget:按钮控件指针；event：事件指针
函数描述:	点击更换皮肤按钮时弹出菜单选项
返回值:         	布尔值
*/
/********************************************/
gboolean doStylePress( GtkWidget *widget,GdkEvent *event )
{
	if (event->type == GDK_BUTTON_PRESS) {
		GdkEventButton *bevent = (GdkEventButton *) event;
		gtk_menu_popup (GTK_MENU(widget), NULL, NULL, NULL, NULL,bevent->button, bevent->time);
		return TRUE;
	}
	return FALSE;
}
/*********************************************/
//处理选中的皮肤选项
/*********************************************/
static void responseToStyleChange(gchar *string)
{
	GdkPixbuf *pixbuf = NULL;
	GdkBitmap *bitmap = NULL;
	GdkPixmap *pixmap = NULL;
	printf ("%s\n",string);
	gtk_widget_set_app_paintable(GTK_WIDGET (window), TRUE);
	gtk_widget_realize(GTK_WIDGET (window));
	pixbuf = createPixbufByName(getStyle(string));      // gdk函数读取png文件
	gdk_pixbuf_render_pixmap_and_mask(pixbuf, &pixmap, &bitmap, 128);   // alpha小于128认为透明
	gtk_widget_shape_combine_mask(GTK_WIDGET (window), bitmap, 0, 0);   // 设置透明蒙板
	gdk_window_set_back_pixmap(window->window, pixmap, FALSE);          // 设置窗口背景
	g_object_unref(pixbuf);
	//g_object_unref(bitmap);
	g_object_unref(pixmap);
	gtk_widget_queue_draw(window);
//	printf ("%s\n",filename);
}
/*******************************************/
/*
函数名:		GtkWidget* getMenu_style(void);
参数:		空
函数描述:	创建皮肤选项菜单
返回值:         	控件指针
*/
/********************************************/
GtkWidget* getStyleMenu(void)
{
	GtkWidget*menu_style,*menu_items_style;
	int i;
	char buf[128];
	menu_style = gtk_menu_new ();
	for (i = 0; i < 5; i++)
	{
	switch(i){
		case 0:
			sprintf (buf, "经典");
			break;
		case 1:
			sprintf (buf, "复古");
			break;
		case 2:
			sprintf (buf, "原始");
			break;
		case 3:
			sprintf (buf, "自然");
			break;
		case 4:
			sprintf (buf, "默认");
			break;
	}
	menu_items_style = gtk_menu_item_new_with_label (buf);
	gtk_menu_shell_append (GTK_MENU_SHELL (menu_style), menu_items_style);
//	g_signal_connect_swapped (G_OBJECT (menu_items_style), "activate",G_CALLBACK (style_response),(gpointer) g_strdup (buf));
	g_signal_connect_swapped (G_OBJECT (menu_items_style), "activate",G_CALLBACK (responseToStyleChange),g_strdup(buf));
	gtk_widget_show (menu_items_style);
	}
	return menu_style;
}

#endif
