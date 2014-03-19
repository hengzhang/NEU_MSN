/********************************************/
/*名称：i_set_soft.h
/*描述： 1、软件设置界面与回调函数的建立
	2、定义维护用户链表的函数
		a. 创建菜单 的选择－－－create_list_model;
        b. 创建 表－－－create_list
        c. 创建 菜单项 －－－create_item;
        d. 创建选择 的动态显示效果－－－on_change;
        e. 创建输入文本 －－－  entry_text;
        f. 调用设置软件的界面－－－ setting;
        g. 输入用户名的回调函数－－－entry_name;
        h. 输入用户组信息的回调函数－－－entry_group;
        i. 输入用户心情信息的回调函数－－－entry_sig;
        j. 设置软件界面的退出回调 函数－－－set_soft_quit;
        k. 定义 设置用户 头像的回调函数 －－－my_callback_function;
/*作者：
/*日期：
/********************************************/
#ifndef I_SETTING_H
#define I_SETTING_H
#include <string.h>
#include <gtk/gtk.h>
enum
{
	PIXBUF_COLUMN,
	TEXT_COLUMN,
	N_COLUMN
};
typedef struct _listitem ListItem;
struct _listitem{
	const gchar *pix;
	const gchar *text;
};
//初始化菜单属性


//static char *filenameimage[10] ={"1.gif","2.gif","3.gif","4.gif","5.gif","6.gif","7.gif","8.gif"};

typedef struct _item_image Item_Image;
struct _item_image{
	 gchar *fcname;
	 gchar *fcimage;
};


/********************************************/
/*
函数名:		create_list_model
参数:		void
函数描述:    创建设置菜单
返回值:         	GtkListStore*
*/
GtkListStore* create_list_model(void);
/********************************************/
/*
函数名:		create_list
参数:		list_store ： 对设置菜单list_store创建 对应的list
函数描述:	 创建list
返回值:         	空
*/
GtkWidget* create_list(GtkListStore *list_store);
/********************************************/
/*
函数名:		create_item
参数:		i ： 对应头像文件编号
函数描述:	 创建 头像选择下拉菜单
返回值:      GtkWidget *
*/
GtkWidget *create_item(gint i);
/********************************************/
/*
函数名:		create_item
参数:		widget ： 调用 的设置界面的item, notebook ：调用传入的参数
函数描述:	 回调函数，点击list对应显示notebook
返回值:      void
*/
void on_change(GtkWidget *widget, gpointer notebook);

void entry_text(GtkWidget *widget,gpointer entry);
/********************************************/
/*
函数名:		setting
参数:		void
函数描述:	 创建 软件设置的界面
返回值:      void
*/
void setting();
/********************************************/
/*
函数名:		entry_name
参数:		widget ： 调用的设置用户姓名的文本框控件,entry ：调用传入的参数
函数描述:	 修改用户姓名的回调函数
返回值:      void
*/
void entry_name(GtkWidget *widget,gpointer entry);
/********************************************/
/*
函数名:		entry_group
参数:		widget ： 调用的设置用户组信息的文本框控件,entry ：调用传入的参数
函数描述:	 修改用户组的回调函数
返回值:      void
*/
void entry_group(GtkWidget *widget,gpointer entry);
/********************************************/
/*
函数名:		entry_sig
参数:		widget ： 调用的设置用户心情的文本框控件,entry ：调用传入的参数
函数描述:	 修改用户心情的回调函数
返回值:      void
*/
void entry_sig(GtkWidget *widget,gpointer entry);
/********************************************/
/*
函数名:		set_soft_quit
参数:		widget ： 调用的设置界面退出的控件,set_soft： 调用的控件
函数描述:	设置界面退出的回调函数
返回值:      void
*/
void set_soft_quit(GtkWidget *widget,GtkWidget *set_soft);
/********************************************/
/*
函数名:		my_callback_function
参数:		widget ： 调用的 修改头像文件的控件,combo： 调用传入的参数
函数描述:	 修改用户头像的回调 函数
返回值:      void
*/
void my_callback_function(GtkWidget *widget,gpointer combo);
#endif
