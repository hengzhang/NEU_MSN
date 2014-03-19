/********************************************/
/*名称：i_textview.h
/*描述：创建文本显示区域，为搜索以及选中项显示其文件内容
/*作者：张珩
/*日期：
/********************************************/
#include<gtk/gtk.h>
#include"i_reminder.h"
#include<stdio.h>


GtkWidget *textview_chat;

//创建一个textiew显示文本内容
GtkWidget *creatTextView(void);

//显示文本内容
//第一个参数是选中的列表项，第二个参数是列表项的IP，用于进行文件名匹配
void displayTextView(GtkWidget *item,gpointer buf_item);

//读文件
//第一个参数是路径，第二个是保存文本显示内容的变量
char* readFile(char * dir, char *buffer);
