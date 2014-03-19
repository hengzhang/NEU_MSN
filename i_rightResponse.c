/************************************************************/
/*
	1、为列表项创建右键菜单及其响应
	

作者：张珩
日期：
*/
/**********************************************************/

#include "i_rightResponse.h"
#include "i_fileChooseDialog.h"
#include "i_textview.h"
#include "i_list.h"

GtkWidget * item_activate;
char* buf_item_activate;

/****************************************************/
/*
函数名:		right_button_press
功能：		右键点击
参数：		GtkWidget * item：控件, GdkEvent * event：事件
返回值:		static gint

*/
/**************************************************/
static gint right_button_press (GtkWidget * item, GdkEvent * event) 
 { 
//	item_activate = item;
  	GdkEventButton *bevent; 
   	if (event->type == GDK_BUTTON_PRESS) 
     	{ 
	       bevent = (GdkEventButton *) event; 
	       if (bevent->button == 3) 
		 { 
		   gtk_menu_popup (GTK_MENU (item), NULL, NULL, NULL, NULL, 
		                   bevent->button, bevent->time); 
		   return TRUE; 
		 } 
     	} 
  
   	return FALSE; 
 }
 
/****************************************************/
/*
函数名:		add_right_menu
功能：		为列表项添加右键菜单
参数：		GtkWidget * item：控件, GtkMenu * menu：传进来的右键菜单
返回值:		void

*/
/**************************************************/
//为控件添加右键菜单
void add_right_menu (GtkWidget * item, GtkMenu * menu) 
 { 
	item_activate = item;
   	g_signal_connect_swapped (G_OBJECT (item), "event", 
                             G_CALLBACK (right_button_press), menu); 
 } 


/****************************************************/
/*
函数名:		right_response
功能：		右键菜单项的响应动作
参数：		gchar *string：响应动作相应的菜单项的名称
返回值:		void

*/
/**************************************************/
void right_response(gchar *string)
{
	char* a[4] = {"导入文件","导出文件","显示最新","清空"};
	if(strcmp(string,a[0])==0)
		create_filechooserdialog();//创建文件选择窗口
	else if(strcmp(string,a[1])==0)
		create_filesavedialog();//创建文件保存窗口
	else if(strcmp(string,a[2])==0)
		displayTextView(item_activate,(gpointer)buf_item_activate);//显示最新的记录
	else if(strcmp(string,a[3])==0)
		gtk_list_clear_items(list_person,1,getCount());//清空列表
	
}

/****************************************************/
/*
函数名:		getMenu_right
功能：		创建右键菜单
参数：		GtkWidget * item：要添加右键菜单的列表项
返回值:		当前控件

*/
/**************************************************/
GtkWidget* getMenu_right(GtkWidget * item,char* buf_item)
{
	GtkWidget *menu_right,*menu_items_right;
	item_activate = item;
	buf_item_activate = buf_item;
	int i;
	char buf[128];
	menu_right = gtk_menu_new ();
        for (i = 0; i < 4; i++)
	{
		switch(i){
			case 0:
				sprintf (buf, "导入文件");
				break;
			case 1:
				sprintf (buf, "导出文件");
				break;
			case 2:
				sprintf (buf, "显示最新");
				break;
			case 3:
				sprintf (buf, "清空");
				break;

			}
//	menu_items_right = gtk_menu_item_new_with_label ("导入文件");
//	gtk_menu_shell_append (GTK_MENU_SHELL (menu_right), menu_items_right);
//	g_signal_connect_swapped (G_OBJECT (menu_items_right), "activate",G_CALLBACK (create_filechooserdialog),(gpointer) g_strdup (buf));
//	gtk_widget_show (menu_items_right);

//	menu_items_right = gtk_menu_item_new_with_label ("导出文件");
//	gtk_menu_shell_append (GTK_MENU_SHELL (menu_right), menu_items_right);
//	g_signal_connect_swapped (G_OBJECT (menu_items_right), "activate",G_CALLBACK (create_filesavedialog),(gpointer) g_strdup (buf));
//	gtk_widget_show (menu_items_right);

//	menu_items_right = gtk_menu_item_new_with_label ("显示");
//	gtk_menu_shell_append (GTK_MENU_SHELL (menu_right), menu_items_right);
//	g_signal_connect_swapped (G_OBJECT (menu_items_right), "activate",G_CALLBACK (displayTextView),(gpointer) g_strdup (buf));
//	gtk_widget_show (menu_items_right);

	menu_items_right = gtk_menu_item_new_with_label (buf);
	gtk_menu_shell_append (GTK_MENU_SHELL (menu_right), menu_items_right);
	g_signal_connect_swapped (G_OBJECT (menu_items_right), "activate",G_CALLBACK (right_response),(gpointer) g_strdup (buf));
	gtk_widget_show (menu_items_right);
	}
	return menu_right;
}

