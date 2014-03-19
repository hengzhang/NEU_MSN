/************************************************************/
/*
	1、创建一个聊天窗口并运行

作者：张珩
日期：
*/
/**********************************************************/


//#include <sys/types.h>
//#include <sys/stat.h>
//#include <unistd.h>
//#include <string.h>
//#include <stdio.h>
//#include <gdk/gdkkeysyms.h>
//#include <gtk/gtk.h>
#include"i_rightResponse.h"
#include"i_list.h"
#include"i_textview.h"
#include"i_search_button.h"
#include"i_entry_IP.h"
//#include"reminder.h"

/****************************************************/
/*
函数名:		getChatRecordWindow
功能：		得到整个聊天记录窗口
参数：		main函数里的参数
返回值:		当前控件

*/
/**************************************************/
void getChatRecordWindow(void)
{
	GtkWidget *window;
	GtkWidget *vbox;
	GtkWidget *hbox_chatRecord;
	GtkWidget *scrolledwindow_person;
	GtkWidget *view;
	GtkWidget *personlist_menu_right;
	GtkWidget *personlist_menu_right_member;
//	GtkWidget *treeview_person;
	GtkWidget *list_person;
	GtkWidget *item;
	GtkWidget *scrolledwindow_text;
	GtkWidget *textview_chat;
	GtkWidget *hbox_search;
	GtkWidget *IP_label;
	GtkWidget *entry_IP;
	GtkWidget *button_search;
	GtkWidget *textview_menu_right;
	GtkWidget *textview_menu_right_member;

//初始化窗口
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (window), "聊天记录");
	g_signal_connect(G_OBJECT(window),"delete_event",G_CALLBACK(gtk_main_quit),NULL);
//纵向盒子，整体布局
  	vbox = gtk_vbox_new (FALSE, 0);
  	gtk_widget_show (vbox);
  	gtk_container_add (GTK_CONTAINER (window), vbox);

  	hbox_chatRecord = gtk_hbox_new (FALSE, 0);
  	gtk_widget_show (hbox_chatRecord);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox_chatRecord, TRUE, TRUE, 0);
//聊天成员滑动窗口
  	scrolledwindow_person = gtk_scrolled_window_new (NULL, NULL);
        gtk_widget_show (scrolledwindow_person);
  	gtk_box_pack_start (GTK_BOX (hbox_chatRecord), scrolledwindow_person, TRUE, TRUE, 0);
 	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolledwindow_person), GTK_SHADOW_IN);
//	gtk_widget_set_size_request (scrolledwindow_person, 300, 300);

// 	treeview_person = gtk_tree_view_new ();
//	gtk_widget_show (treeview_person);
//  	gtk_container_add (GTK_CONTAINER (scrolledwindow_person), treeview_person);
//	gtk_widget_set_size_request (treeview_person, 300, 300);

//创建观察框
	//视区控件GtkViewPort,它也是一个容器(只能容纳一个控件)，在功能上和框架大致相同,主要是美化控件外观的作用
	//两个参数用来描述可调整的边界的值，默认时用NULL代替
	view = gtk_viewport_new(NULL,NULL);
	gtk_container_add(GTK_CONTAINER(scrolledwindow_person),view);
//	gtk_box_pack_start(GTK_BOX(scrolledwindow_person),view,TRUE,TRUE,5);
	gtk_widget_set_size_request (view, 280, 420);
//创建列表
	list_person = creatList();
	gtk_container_add(GTK_CONTAINER(view),list_person);

	//初始化列表项顶，无选中及右键菜单
	item=gtk_list_item_new_with_label("IP          		");
	gtk_container_add(GTK_CONTAINER(list_person),item);
	gtk_widget_show(item);
//	list_add(NULL,NULL);
	int i;
	char* IP[3] = {{"10.10.31.1"},{"10.10.31.2"},{"10.10.31.3"}};
	for(i = 0; i < 3; i++)
	{
		list_add(NULL,IP[i]);
	}
        //设置列表框的选择模式(单选或多选)
	gtk_list_set_selection_mode(GTK_LIST(list_person),GTK_SELECTION_SINGLE);

//文本滑动窗口
  	scrolledwindow_text = gtk_scrolled_window_new (NULL, NULL);
  	gtk_widget_show (scrolledwindow_text);
  	gtk_box_pack_start (GTK_BOX (hbox_chatRecord), scrolledwindow_text, TRUE, TRUE, 0);
  	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolledwindow_text), GTK_SHADOW_IN);
//显示聊天文本域
  	textview_chat = creatTextView();
//  	gtk_widget_show (textview_chat);
  	gtk_container_add (GTK_CONTAINER (scrolledwindow_text), textview_chat);
//  	gtk_widget_set_size_request (textview_chat, 350, 420);
//	gtk_text_view_set_editable (GTK_TEXT_VIEW (textview_chat), FALSE);
//	gtk_text_buffer_set_text (gtk_text_view_get_buffer (GTK_TEXT_VIEW (textview_chat)),"此处显示文本", -1);
//横向的搜索区域
  	hbox_search = gtk_hbox_new (FALSE, 0);
  	gtk_widget_show (hbox_search);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox_search, TRUE, TRUE, 0);
//IP标签
	IP_label = gtk_label_new("用户IP:");
	gtk_box_pack_start (GTK_BOX (hbox_search), IP_label, FALSE, FALSE, 5);
//IP输入框
	entry_IP = creatEntry_IP();
 	gtk_box_pack_start (GTK_BOX (hbox_search), entry_IP, TRUE, TRUE, 0);
// 	gtk_entry_set_text (GTK_ENTRY (entry_IP), search_IP);
//	g_signal_connect(G_OBJECT(entry_IP),"toggle_overwrite",G_CALLBACK(list_add),NULL);
//  	gtk_entry_set_invisible_char (GTK_ENTRY (entry_IP), 9679);
//	search_IP = gtk_entry_get_text(entry_IP);		
//搜索按钮
	button_search = creat_search_button();
// 	button_search = gtk_button_new_with_mnemonic ("搜索");
//	gtk_widget_show (button_search);
//	g_signal_connect(G_OBJECT(button_search),"clicked",G_CALLBACK(list_add),NULL);
  	gtk_box_pack_start (GTK_BOX (hbox_search), button_search, FALSE, FALSE, 0);
//  	gtk_widget_set_size_request (button_search, 50, 50);
	gtk_widget_show_all(window);
	//gtk_main();
}
/*int main(int argc, char*argv[])
{
	GtkWidget *window;
	window = getChatRecordWindow(&argc,&argv);
	return FALSE;
}*/
