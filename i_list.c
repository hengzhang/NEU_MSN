/************************************************************/
/*
	1、创建一个列表
	

作者：张珩
日期：
*/
/**********************************************************/
#include"i_list.h"

static gint count_person = 1;
GtkWidget *personlist_menu_right;
GtkWidget *personlist_menu_right_member;

/****************************************************/
/*
函数名:		getCount
功能：		得到列表项数，以便清空
参数：		void
返回值:		gint ：列表项数

*/
/**************************************************/
gint getCount()
{
	return count_person;
}

/****************************************************/
/*
函数名:		creatList
功能：		创建一个列表
参数：		void
返回值:		当前控件

*/
/**************************************************/
GtkWidget * creatList(void)
{
	list_person = gtk_list_new();
	return list_person;
}

/****************************************************/
/*
函数名:		creatList
功能：		往列表里添加列表项
参数：		GtkWidget *widget:调用的控件, gpointer data：用户自定义数据
返回值:		void

*/
/**************************************************/
void list_add(GtkWidget *widget,gpointer data)
{	count_person++;
	GtkWidget *item;
	char* buf_item;
//	const int i = count_person;
	buf_item = (char*)data;

//创建一个列表项
	item = gtk_list_item_new_with_label(buf_item);
	gtk_container_add(GTK_CONTAINER(list_person),item);
	gtk_widget_show(item);
//选中显示文本
	g_signal_connect(G_OBJECT(item),"select",G_CALLBACK(displayTextView),(gpointer)buf_item);
//创建列表项右键菜单
	personlist_menu_right = getMenu_right(item,buf_item);
	add_right_menu (item,GTK_MENU(personlist_menu_right));	
//	count_person++;
}
/*删除成员记录
void list_clear(GtkWidget *widget,gpointer data)
{
	
}

//搜索成员函数
void search(GtkWidget *widget,gpointer data)
{

}*/

