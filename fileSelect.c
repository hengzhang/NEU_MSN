/********************************************/
/*名称：fileSelect.c
/*描述：此文件定义了文件选择的相关函数
/*作者：
/*日期：
/********************************************/


#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <time.h>
#include <assert.h>
#include <gdk/gdkkeysyms.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>


#include "i_setting.h"
#include "filetansfer.h"
#include "i_utils.h"
#include "g_properties.h"
#include "i_chat_window.h"
#include "i_chat.h"

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#define GLADE_HOOKUP_OBJECT(component,widget,name) \
  g_object_set_data_full (G_OBJECT (component), name, \
    gtk_widget_ref (widget), (GDestroyNotify) gtk_widget_unref)

#define GLADE_HOOKUP_OBJECT_NO_REF(component,widget,name) \
  g_object_set_data (G_OBJECT (component), name, widget)


/*****用户ip*****/
char* ip ;

/*函数声明，定义见本文件*/
void destroy(GtkWidget *widget,GtkFileSelection *fs);

/**************************************************/
/*函数名：createFileSelect
/*功能：   创建文件选择界面
/*日期： 
/*参数：void
/*返回值： GtkWidget* :指向文件选择界面的结构体
/*作者：
/***************************************************/
GtkWidget *createFileSelect()
{
  GtkWidget *fileselection;
  GtkWidget *ok_button;
  GtkWidget *cancel_button;
  printf("new fileselection window \n");
  fileselection = gtk_file_selection_new ("send file");
  if(NULL==fileselection){
	printf("new fileselection windows faile\n");
	return NULL ;
  }
  printf("new finish\n");
  gtk_container_set_border_width (GTK_CONTAINER (fileselection), 10);
  gtk_window_set_type_hint (GTK_WINDOW (fileselection), GDK_WINDOW_TYPE_HINT_DIALOG);
  printf("set fileselection window \n");
  ok_button = GTK_FILE_SELECTION (fileselection)->ok_button;
  gtk_widget_show (ok_button);
  GTK_WIDGET_SET_FLAGS (ok_button, GTK_CAN_DEFAULT);

  cancel_button = GTK_FILE_SELECTION (fileselection)->cancel_button;
  gtk_widget_show (cancel_button);
  GTK_WIDGET_SET_FLAGS (cancel_button, GTK_CAN_DEFAULT);
 
  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (fileselection, fileselection, "fileselection");
  GLADE_HOOKUP_OBJECT_NO_REF (fileselection, ok_button, "ok_button");
  GLADE_HOOKUP_OBJECT_NO_REF (fileselection, cancel_button, "cancel_button");
  g_signal_connect (G_OBJECT (cancel_button),"clicked",G_CALLBACK (destroy),fileselection);
  printf("return fileselection windows");
  return fileselection;
}

/**************************************************/
/*函数名：file_ok_sel
/*功能：   获取文件选择路径，并发送
/*日期： 
/*参数：
         GtkWidget *button ：按钮控件指针
         GtkFileSelection *fs :文件选择控件
/*返回值： void
/*作者：
/***************************************************/
void file_ok_sel(GtkWidget *button, GtkFileSelection *fs)
{ 

 
   LinpopChat chat;
   struct sockaddr_in chatAddr;
   inet_aton(ip,&chatAddr.sin_addr);
   gboolean result;
   char *file_path = gtk_file_selection_get_filename (GTK_FILE_SELECTION (fs)); 
   printf("get file name success\n");
   printf ("the file name is :%s\n",file_path);
   if(NULL==file_path)
   {	
	printf("get filename faile");
        return 0 ;
   }

   file_pthread_t file ;
   file.path = file_path ;
   memset(&(file.client),0,sizeof(struct sockaddr_in));
   printf("begin set socket \n");
   file.client.sin_family = AF_INET;
   printf(" step one success\n");
   file.client.sin_addr.s_addr = /*inet_addr*/inet_addr(ip);/*设置对方的ip*/
   printf("the ip is: %s\n",ip);
   printf(" step two success\n");
   file.client.sin_port =htons(9734) ;
   printf(" step three success\n");

   result = getChatByIP(chatAddr,&chat);
   assert(result == TRUE);
   printInfoToTextView(chat.showTextView,option.user,"File Send OK!");
   file_send(&file);
  
}
/**************************************************/
/*函数名：fileSelect
/*功能：   显示文件选择界面，并设置回调函数
/*日期： 
/*参数：
         GtkWidget *button ：按钮控件指针
         gpointer  data :传入的数据
/*返回值： void
/*作者：
/***************************************************/

void fileSelect(GtkWidget *button, gpointer data)
{
   GtkWidget *filew ;
   /*创建文件选择窗口*/
   printf("begin create file select windows\n");
   filew = createFileSelect();
   if(NULL==filew)
   { 
	printf("window not found\n");
   }
   
   else{
     printf("create windwo success !\n");
    /*设置文件传输的socket地址内容*/
      ip = data ;
     /* 设置选择文件结束后的确定回调函数 */
    g_signal_connect (G_OBJECT (GTK_FILE_SELECTION (filew)->ok_button),
		      "clicked", 
                      G_CALLBACK (file_ok_sel),filew);
   
    printf("connect signal success !\n");
    gtk_widget_show (filew);
    }
}

/**************************************************/
/*函数名：destroy
/*功能：   销毁文件选择窗口
/*日期： 
/*参数：
	GtkWidget *widget:控件指针
	GtkFileSelection *fs:文件选择器指针
/*返回值：void
/*作者：
/***************************************************/
void destroy(GtkWidget *widget,GtkFileSelection *fs){
    gtk_widget_destroy(fs);
}


