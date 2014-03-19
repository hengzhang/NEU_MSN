/************************************************************/
/*
	1、创建文本显示区域，为搜索以及选中项显示其文件内容
	

作者：张珩
日期：
*/
/**********************************************************/

#include"i_textview.h"
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>

//static int i;

/****************************************************/
/*
函数名:		creatTextView
功能：		创建文本显示域
参数：		void
返回值:		创建的文本显示域控件

*/
/**************************************************/
GtkWidget *creatTextView(void)
{
//	GtkWidget *textview_chat;
	textview_chat = gtk_text_view_new ();
//	buf = "聊天记录：\n";
  	gtk_widget_show (textview_chat);
//  	gtk_container_add (GTK_CONTAINER (scrolledwindow_text), textview_chat);
  	gtk_widget_set_size_request (textview_chat, 350, 420);
	gtk_text_view_set_editable (GTK_TEXT_VIEW (textview_chat), FALSE);
	gtk_text_buffer_set_text (gtk_text_view_get_buffer (GTK_TEXT_VIEW (textview_chat)),"聊天记录文本区", -1);
	return textview_chat;
}


/****************************************************/
/*
函数名:		displayTextView
功能：		显示文件内容
参数：		GtkWidget *item：选中项, gpointer buf_item：保存选中项或者搜索成功后的用户自定义类型的IP值与文件名进行匹配
返回值:		void
*/
/**************************************************/
void displayTextView(GtkWidget *item,gpointer buf_item)
{
	char buffer[102400]={0};//用于整个文本的缓冲,初始化保证末尾为结束符'\0'
//	readFile("/home/neusoft/桌面/chatRecord_HJB/1.txt",buffer);//读取文件中的多行到buffer中
	readFile((char*)buf_item,buffer);
	gtk_text_buffer_set_text (gtk_text_view_get_buffer (GTK_TEXT_VIEW (textview_chat)),buffer, -1);

}


/****************************************************/
/*
函数名:		readFile
功能：		读文件
参数：		char * dir：路径, char *buffer：保存文件内容的缓存
返回值:		char＊：文件内容
*/
/**************************************************/
char* readFile(char * dir, char *buffer){
	
    	FILE * fp;
	char temp_buf[256] = {0};//用于读取一行文本的缓冲,初始化保证末尾为结束符'\0'
    	if((fp=fopen(dir,"r"))==NULL)//在查找不到IP，与文件名对应不了的时候在display()函数里写进一个提示窗口
    	{
		g_print("3\n");
		g_print("Cannot open file strike any key exit!");
		//在查找不到IP，与文件名对应不了的时候在display()函数里写进一个提示窗口
		GtkWidget *reminder;
		reminder = create_reminder_window();
   		gtk_widget_show(reminder);
  	}
	else
	{
		while(fgets(temp_buf, 256, fp))
		{//每次读一行到temp_buf中，直到EOF
				strcat(buffer,temp_buf);//追加temp_buf到buffer中
		}
		fclose(fp);
    	}
    	return buffer;
}


