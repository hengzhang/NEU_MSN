#ifndef __I_UTILS_
#define __I_UTILS_
#include <gtk/gtk.h>
#include "i_main_callbacks.h"

/**************************************************/
/*名称：createImageLabelBox
/*描述：创建图片标签盒子
/*作成日期：
/*参数：
        gchar *imageName：图片名
        gchar *labelText：标签内容
/*返回值：void
/*作者：
/***************************************************/
GtkWidget *createImageLabelBox(gchar *imageName,gchar *labelText){
    GtkWidget *box;
    GtkWidget *label;
    GtkWidget *image;

    box = gtk_hbox_new (FALSE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (box), 2);

    image = gtk_image_new_from_file (imageName);

    label = gtk_label_new (labelText);

    gtk_box_pack_start (GTK_BOX (box), image, FALSE, FALSE, 3);
    gtk_box_pack_start (GTK_BOX (box), label, FALSE, FALSE, 3);

    gtk_widget_show (image);
    gtk_widget_show (label);

    return box;
}

/**************************************************/
/*名称：createPixbufByName
/*描述：创建位图
/*作成日期：
/*参数：
        const gchar * filename：文件名
/*返回值：void
/*作者：
/***************************************************/
GdkPixbuf *createPixbufByName(const gchar * filename){
	GdkPixbuf *pixbuf;
	GError *error = NULL;
	pixbuf = gdk_pixbuf_new_from_file(filename, &error);
	if(!pixbuf) {
	fprintf(stderr, "%s\n", error->message);
	g_error_free(error);
	}
	return pixbuf;
}

/**************************************************/
/*名称：showTimeoutMessage
/*描述：显示超时信息
/*作成日期：
/*参数：
        const gchar *msg：信息
/*返回值：void
/*作者：
/***************************************************/
void showTimeoutMessage(const gchar *msg)
{
  GtkWidget *dialog1;
  GtkWidget *dialog_vbox1;
  GtkWidget *label1;
  GtkWidget *dialog_action_area1;

  dialog1 = gtk_dialog_new ();
  gtk_window_set_title (GTK_WINDOW (dialog1), "提示");
  gtk_window_set_type_hint (GTK_WINDOW (dialog1), GDK_WINDOW_TYPE_HINT_DIALOG);

  dialog_vbox1 = GTK_DIALOG (dialog1)->vbox;
  gtk_widget_show (dialog_vbox1);

  label1 = gtk_label_new (msg);
  gtk_widget_show (label1);
  gtk_box_pack_start (GTK_BOX (dialog_vbox1), label1, FALSE, FALSE, 0);
  gtk_widget_set_size_request (label1, -1, 130);

  dialog_action_area1 = GTK_DIALOG (dialog1)->action_area;
  gtk_widget_show (dialog_action_area1);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area1), GTK_BUTTONBOX_END);

  gtk_widget_show_all(dialog1);
  g_timeout_add(2000,closeTimeoutWindow,dialog1);
}

/**************************************************/
/*名称：createImageButton
/*描述：创建图片按钮
/*作成日期：
/*参数：
        gchar *filename：文件名
/*返回值：GtkWidget *
/*作者：
/***************************************************/
GtkWidget *createImageButton( gchar *filename)
{
	GtkWidget *image;
	GtkWidget *button;
	button = gtk_button_new ();
	image = gtk_image_new_from_file (filename);	
	gtk_container_add (GTK_CONTAINER (button), image);
	gtk_container_set_border_width (GTK_CONTAINER (button), 0);
	return button;
}

/**************************************************/
/*名称：getCurrentDay
/*描述：获取当前日期
/*作成日期：
/*参数：
         参数1：char *buf
         参数2: int size
/*返回值：void
/*作者：
/***************************************************/
void getCurrentDay(char  buf[],int size)
{
	time_t timep;
	struct tm * p;
	time(&timep);
	p = localtime(&timep);
	strftime(buf,size,"%Y%m%d",p);
}

/**************************************************/
/*名称：getCurrentTime
/*描述：获取当前时间
/*作成日期：
/*参数：
         参数1：char *buf
         参数2: int size
/*返回值：void
/*作者：
/***************************************************/
void getCurrentTime(char buf[],int size)
{
	time_t timep;
	struct tm * p;
	time(&timep);
	p = localtime(&timep);
	strftime(buf,size,"%Y-%m-%d    %H:%M:%S",p);
	printf("%s\n",buf);
}	

/**************************************************/
/*名称：numToStr
/*描述：将数字转换为字符串
/*作成日期：
/*参数：
         参数1：char *buf
         参数2: int num
/*返回值：void
/*作者：
/***************************************************/
void numToStr(char *buf,int num){
    int i,m;
    char stack[15];
    bzero(stack,sizeof(stack));
    for(i = 0;num;i++,num /= 10){
        m = num %10;
	stack[i] = m + 48;
    }
    for(--i,m = 0;i >= 0;i--,m++){
        buf[m] = stack[i] ;
    }
    buf[m] = '\0';
}

/**************************************************/
/*名称：trayIconActivated
/*描述：激活图标
/*作成日期：
/*参数：
         参数1：GObject *trayIcon
         参数2: gpointer window
/*返回值：void
/*作者：
/***************************************************/
/*void trayIconActivated(GObject *trayIcon, gpointer window)
{
	if(gtk_status_icon_get_blinking(GTK_STATUS_ICON (trayIcon))==FALSE){
		gtk_widget_show(GTK_WIDGET(window));
		gtk_window_deiconify(GTK_WINDOW(window));
	}
}*/

/**************************************************/
/*名称：die
/*描述：出错处理函数，打印出错信息，并退出程序。
/*作成日期： 2010-06-26
/*参数：
         参数1：message、char ＊、输入参数、错误信息的指针
/*返回值：void
/*作者：
/***************************************************/
void die(char * message)
{
	perror(message);
	
}



/**************************************************/
/*名称：copyData
/*描述：复制文件数据
/*作成日期： 2010-06-26
/*参数：
         参数1：form、int、输入参数、源文件文件描述符号
         参数2：to、int、输入参数、目的文件文件描述符号
/*返回值：void
/*作者：
/***************************************************/
void copyData(int from, int to)
{
	char buf[512];
	int amount;
	
	while(( amount = recv(from,buf,sizeof(buf))) > 0)
	{
		if( write(to,buf,amount) != amount )
		{
			die("write");
			return;
		}
	}

	if(amount < 0)
		die("read");
}
#endif
