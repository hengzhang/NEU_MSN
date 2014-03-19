/************************************************************/
/*
	1、创建一个单行文本输入框

作者：张珩
日期：
*/
/**********************************************************/
#include"i_entry_IP.h"


/****************************************************/
/*
函数名:		creatEntry_IP
功能：		创建一个单行文本输入框
参数：		void
返回值:		当前控件

*/
/**************************************************/
GtkWidget *creatEntry_IP(void)
{
	
  	entry_IP = gtk_entry_new ();
	entry_IP = gtk_entry_new_with_max_length (20);
	gtk_entry_set_editable(entry_IP,TRUE);
  	gtk_widget_set_size_request (entry_IP, 200, 40);
	gtk_widget_show (entry_IP);
	return entry_IP;
}
