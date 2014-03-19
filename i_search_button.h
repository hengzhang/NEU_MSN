/********************************************/
/*名称：search_button.h
/*描述：为文本输入框创建搜索按钮及其响应
/*作者：张珩
/*日期：
/********************************************/


#include<gtk/gtk.h>
#include"i_entry_IP.h"
#include"i_textview.h"
#include"i_list.h"

//创建搜索按钮
GtkWidget *creat_search_button(void);
//点击按钮时的动作，调用displayview()函数
void on_clicked(void);
