/********************************************/
/*名称：i_list.h
/*描述：创建一个列表
/*作者：张珩
/*日期：
/********************************************/
#include <gtk/gtk.h>
#include"i_textview.h"

//在清空时候需要调用列表
GtkWidget *list_person;

//创建列表
GtkWidget * creatList(void);

//得到列表项数
gint getCount();

//往列表里添加列表项 
void list_add(GtkWidget *widget,gpointer data);


//void list_clear(GtkWidget *widget,gpointer data);
//void search(GtkWidget *widget,gpointer data);

