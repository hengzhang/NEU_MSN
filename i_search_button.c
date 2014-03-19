/************************************************************/
/*
	1、为文本输入框创建搜索按钮及其响应
	

作者：张珩
日期：
*/
/**********************************************************/
#include"i_search_button.h"


/****************************************************/
/*
函数名:		creat_search_button
功能：		创建按钮
参数：		void
返回值:		创建的按钮控件

*/
/**************************************************/
GtkWidget *creat_search_button(void)
{
	GtkWidget *button_search;
	button_search = gtk_button_new_with_mnemonic ("搜索");
  	gtk_widget_set_size_request (button_search, 50, 50);
	gtk_widget_show (button_search);
	g_signal_connect(G_OBJECT(button_search),"clicked",G_CALLBACK(on_clicked),NULL);
//	if(g_signal_connect(G_OBJECT(button_search),"clicked",G_CALLBACK(list_add),IP[i]))
//		i++;
	
	return button_search;
}


/****************************************************/
/*
函数名:		on_clicked
功能：		点击后显示搜索到的结果
参数：		void
返回值:		void

*/
/**************************************************/
void on_clicked()
{
	const gchar *IP;
	IP = gtk_entry_get_text(entry_IP);
	displayTextView(NULL,IP);
}
