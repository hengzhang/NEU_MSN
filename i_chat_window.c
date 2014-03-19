#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <time.h>
#include <pthread.h>
#include <netinet/in.h>
#include <gdk/gdkkeysyms.h>
#include "i_chat_window.h"
#include "filetansfer.h"
#include "i_utils.h"
#include "i_chat_callbacks.h"
#include "i_addFace.h"
#include "i_calendar.h"
#include "i_fontSelect.h"
#include "filetansfer.h"
#include "i_setting.h"

static void getStrTime(gchar *buf,guint n);
static int file_thread(void);
/******************************************************************
创建list的模型
*******************************************************************/
extern struct _item_image ;

struct _item_image  image[2] = {{"图像1","pic/333.JPG"},{"图像2","pic/222.JPG"}};
/**************************************************************/


/**************************************************************/
void changePersonFace(void* argv);
void on_select(GtkWidget *widget);
void widget_destroy(GtkWidget *widget,GtkWidget *window);
/************************************************************************/


/**************************************************/
/*函数名：clearTextView
/*功能：清空文本区
/*时间：
/*参数：
        GtkWidget *textView
/*返回值：void
/*作者：
/***************************************************/
void clearTextView(GtkWidget *textView){
    GtkTextIter start,end;
    GtkTextBuffer *buffer;
    buffer = gtk_text_view_get_buffer(textView);
    gtk_text_buffer_get_bounds(buffer,&start,&end);
    gtk_text_buffer_delete(buffer,&start,&end);
    return ;
}

/**************************************************/
/*函数名：getTextFromTextView
/*功能：获取聊天窗口中的信息
/*时间：
/*参数：
         GtkWidget *textView:控件指针
         gchar *buf: 用户
	 guint n :
/*返回值：void
/*作者：
/***************************************************/
void getTextFromTextView(GtkWidget *textView,gchar *buf,guint n){
    GtkTextIter start,end;
    GtkTextBuffer *buffer;
    gchar *text;
    buffer = gtk_text_view_get_buffer(textView);
    gtk_text_buffer_get_bounds(buffer,&start,&end);
    text = gtk_text_buffer_get_text(buffer,&start,&end,FALSE);
    g_print("text is:%s\n",text);
    strncpy(buf,text,n);
    gtk_text_buffer_delete(buffer,&start,&end);
    return ;
}

/**************************************************/
/*函数名：printInfoToTextView
/*功能：在聊天窗口中打印信息
/*时间：
/*参数：
         GtkWidget *textView:控件指针
         gchar *user: 用户
	 gchar *info:信息
/*返回值：void
/*作者：
/***************************************************/
void printMsgToTextView(GtkWidget *textView,gchar *user,gchar *content){
    GtkTextIter start,end;
    GtkTextChildAnchor *anchor;
    GtkWidget *image;
    char buf[1024];
    char num = 0;
    char temp[2048] ;
    size_t size;
    int i;
    bzero(temp,sizeof(temp));
    size = strlen(user);
    strcpy(buf,user);
    getStrTime(buf + size ,sizeof(buf) - size);
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(textView);
    gtk_text_buffer_get_bounds(buffer,&start,&end);
    gtk_text_buffer_insert_with_tags_by_name(buffer,&end,buf,-1,"blue_foreground",NULL);

    for(i = 0; *content;content++,i++){
        if(*content == '\\'){
	    if(*(content + 1) == '\\'){
	        temp[i] = '\\';
	        content++;
	    }else{
	        content++;
		num = *content;
		gtk_text_buffer_insert(buffer,&end,temp,-1);
		i = -1;
		bzero(temp,sizeof(temp));
		image = getImageByNum(num);
		g_print("image:%x\n",image);
		anchor = gtk_text_buffer_create_child_anchor(buffer,&end);
		gtk_text_view_add_child_at_anchor(textView,image,anchor);
		gtk_widget_show(image);
	    }
	}else{
	    temp[i] = *content;
	}
    }
    if(temp[0] != '\0')
        gtk_text_buffer_insert(buffer,&end,temp,-1);

    gtk_text_buffer_insert(buffer,&end,"\n",-1);
    return ;
}

/**************************************************/
/*函数名：printInfoToTextView
/*功能：在聊天窗口中打印信息
/*时间：
/*参数：
         GtkWidget *textView:控件指针
         gchar *user: 用户
	 gchar *info:信息
/*返回值：void
/*作者：
/***************************************************/
void printInfoToTextView(GtkWidget *textView,gchar *user,gchar *info){
    GtkTextIter start,end;
    const char *split1 = "************";
    const char *split2 = "*******************************\n";
    char buf[1024];
    size_t size;
    size = strlen(user);
    strcpy(buf,user);
    getStrTime(buf + size ,sizeof(buf) - size);
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(textView);
    gtk_text_buffer_get_bounds(buffer,&start,&end);
    gtk_text_buffer_insert_with_tags_by_name(buffer,&end,split1,-1,"red_foreground",NULL);
    gtk_text_buffer_insert_with_tags_by_name(buffer,&end,user,-1,"green_foreground",NULL);
    gtk_text_buffer_insert_with_tags_by_name(buffer,&end,split1,-1,"red_foreground",NULL);
    gtk_text_buffer_insert(buffer,&end,"\n",-1);
    gtk_text_buffer_insert_with_tags_by_name(buffer,&end,info,-1,"blue_foreground",NULL);
    gtk_text_buffer_insert(buffer,&end,"\n",-1);
    gtk_text_buffer_insert_with_tags_by_name(buffer,&end,split2,-1,"red_foreground",NULL);
    return ;
}


/**************************************************/
/*函数名：printEchoToTextView
/*功能：在聊天窗口中打印返回信息
/*时间：
/*参数：
         GtkWidget *textView:控件指针
         gchar *user: 用户
	 gchar *content:内容
/*返回值：void
/*作者：
/***************************************************/
void printEchoToTextView(GtkWidget *textView,gchar *user,gchar *content){
    GtkTextIter start,end;
    GtkTextChildAnchor *anchor;
    GtkWidget *image;
    char buf[1024];
    char num = 0;
    char temp[2048] ;
    size_t size;
    int i;
    bzero(temp,sizeof(temp));
    size = strlen(user);
    strcpy(buf,user);
    getStrTime(buf + size ,sizeof(buf) - size);
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(textView);
    gtk_text_buffer_get_bounds(buffer,&start,&end);
    gtk_text_buffer_insert_with_tags_by_name(buffer,&end,buf,-1,"green_foreground",NULL);
    for(i = 0; *content;content++,i++){
        if(*content == '\\'){
	    if(*(content + 1) == '\\'){
	        temp[i] = '\\';
	        content++;
	    }else{
	        content++;
		num = *content;
		gtk_text_buffer_insert(buffer,&end,temp,-1);
		i = -1;
		bzero(temp,sizeof(temp));
		image = getImageByNum(num);
		anchor = gtk_text_buffer_create_child_anchor(buffer,&end);
		gtk_text_view_add_child_at_anchor(textView,image,anchor);
		gtk_widget_show(image);
	    }
	}else{
	    temp[i] = *content;
	}
    }
    if(temp[0] != '\0')
        gtk_text_buffer_insert(buffer,&end,temp,-1);
    //gtk_text_buffer_insert(buffer,&end,content,-1);
    gtk_text_buffer_insert(buffer,&end,"\n",-1);
    return ;
}

/**************************************************/
/*函数名：createchatWindow
/*功能：创建聊天界面
/*时间：
/*参数：
         char* getip :ip地址
         char* getname :姓名
	 int    mode:  
/*返回值：void
/*作者：
/***************************************************/
void createChatWindow(char *getip,char *getname,int mode)
{
  char *ip = g_strdup(getip);
 
  char name[256];
  gchar title[256];
  struct sockaddr_in addr;
  gboolean result;
  LinpopChat chat;
  GtkTextIter iter;
  
  /*创建显示窗体指针*/
  GtkWidget *mainWindow;
  GtkWidget *showTextView;
  GtkWidget *inputTextView;
  GtkWidget *selfImage;
  GtkWidget *otherImage;
  GtkWidget *selfLabel;
  GtkWidget *otherLabel;
  GtkWidget *scrolled_window1,*scrolled_window2;
  GtkAccelGroup *gag; 
  
  /*声明控件变量*/
  GtkWidget *button1,*button2,*button3,*button4,*button5,*button6,*button7,*button8,*button9,*button10,*button11,*button12;
  GtkWidget *label_button1, *label_button2, *label_button3, *label_button4, *label_button5, *label_button8,*label_button9,*label_button10,*label_button11,*label_button12;
  GtkWidget *label_name, *label_ip ;
  GtkWidget *hbox1,*hbox2,*hbox3,*hbox4;
  GtkWidget *vbox1,*vbox2,*vbox3,*vbox4,*vbox5,*vbox6,*vbox7;
  GtkWidget *image1, *image2 ;
  GtkWidget *separator;
  inet_aton(ip,&addr.sin_addr);
  result = getChatByIP(addr,&chat);
  if(result == TRUE){
      g_print("have window\n");
      return ;
  }
  mainWindow= gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_icon(GTK_WINDOW ( mainWindow),createPixbufByName("pic/logo.bmp"));
  //bzero(title,sizeof(title));
  strcpy(title,"Chat with:");
  //strcpy(title,name); 
  //strcat(title,""); 
  strcat(title,ip); 
  //strcat(title,")"); 
  //g_print("title is:%s\n",title);
  gtk_window_set_resizable (GTK_WINDOW ( mainWindow), FALSE);

 /************************************/
  //
  //添加背景
  /************************************/
	GdkPixbuf *pixbuf = NULL;
	GdkBitmap *bitmap = NULL;
	GdkPixmap *pixmap = NULL;
	gtk_widget_set_app_paintable(GTK_WIDGET (mainWindow), TRUE);
	gtk_widget_realize(GTK_WIDGET (mainWindow));
	pixbuf = createPixbufByName("Icon/026.jpg");      // gdk函数读取png文件
	gdk_pixbuf_render_pixmap_and_mask(pixbuf, &pixmap, &bitmap, 128);   // alpha小于128认为透明
	gtk_widget_shape_combine_mask(GTK_WIDGET (mainWindow), bitmap, 0, 0);   // 设置透明蒙板
	gdk_window_set_back_pixmap(mainWindow->window, pixmap, FALSE);          // 设置窗口背景
	g_object_unref(pixbuf);
	//g_object_unref(bitmap);
	g_object_unref(pixmap);
/************************************/
//the end
/************************************/
  scrolled_window1 = gtk_scrolled_window_new(NULL,NULL);
  gtk_container_set_border_width(GTK_CONTAINER(scrolled_window1),5);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window1),GTK_POLICY_NEVER,GTK_POLICY_AUTOMATIC);
  scrolled_window2 = gtk_scrolled_window_new(NULL,NULL);
  gtk_container_set_border_width(GTK_CONTAINER(scrolled_window2),5);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window2),GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);

 /*设置窗口属性*/
  gtk_window_set_title(GTK_WINDOW(mainWindow ),title);
  gtk_window_set_position(GTK_WINDOW(mainWindow ), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(mainWindow ), 600, 475);
/*设置窗体宽度*/
  gtk_container_set_border_width(GTK_CONTAINER(mainWindow ),2);

   /*连接关闭窗口的自定义回调函数*/
  g_signal_connect ( G_OBJECT(mainWindow), "destroy",closeChatWindow,ip);
  printf("------------!6\n");

  /*创建vbox*/
  vbox1 = gtk_vbox_new(FALSE,0);
  vbox2 = gtk_vbox_new(FALSE,0);
  vbox3 = gtk_vbox_new(FALSE,0);
  vbox4 = gtk_vbox_new(TRUE,0);
  vbox5 = gtk_vbox_new(FALSE,0);
  vbox6 = gtk_vbox_new(FALSE,0);
  vbox7 = gtk_vbox_new(FALSE,0);

  hbox1 = gtk_hbox_new(FALSE,0);
  hbox2 = gtk_hbox_new(FALSE,0);
  hbox3 = gtk_hbox_new(FALSE,0);
  hbox4 = gtk_hbox_new(FALSE,0);

  g_print("box new OK\n");
  selfImage = gtk_image_new_from_file ("pic/222.JPG");
  otherImage = gtk_image_new_from_file ("pic/333.JPG");
  g_print("image new OK\n");

  /*设置人物信息*/
  selfLabel = gtk_label_new("本人形象");
  otherLabel = gtk_label_new("对方形象");
  label_name = gtk_label_new(getname);
  label_ip   = gtk_label_new(getip);

 /********设置人物形象*********/
  buttonMyFace = gtk_button_new();
  buttonHiFace = gtk_button_new();
  image1 = gtk_image_new_from_file ("pic/222.JPG");
  image2 = gtk_image_new_from_file ("pic/333.JPG");
  gtk_button_set_image(GTK_BUTTON(buttonMyFace),image1);
  gtk_button_set_image(GTK_BUTTON(buttonHiFace),image2);
  gtk_signal_connect(GTK_OBJECT (buttonMyFace), "clicked", G_CALLBACK(changePersonFace), NULL);

  g_print("label new OK\n");
/*创建按钮控件*/
  button1 = gtk_button_new();
  button2 = gtk_button_new();
  button3 = gtk_button_new();
  button4 = gtk_button_new();
  button5 = gtk_button_new();
  button8 = gtk_button_new();
  button9 = gtk_button_new();
  button10 = gtk_button_new();
  button11 = gtk_button_new();
  button12 = gtk_button_new();
  g_print("button new OK\n");
 


 /* 初始化图片按钮 */  
    label_button1 = createImageLabelBox ("pic/changeImage.png", "形象");
    label_button2 = createImageLabelBox ("pic/changeFace.png", "皮肤");
    label_button3 = createImageLabelBox ("pic/file.png", "文件");
    label_button4 = createImageLabelBox ("pic/font.png", "字体");
    label_button5 = createImageLabelBox ("pic/face.png", "表情");
    label_button8 = createImageLabelBox ("pic/calendar.png", "日历");
    label_button9 = createImageLabelBox ("pic/chatRecord.png", "记录");
    label_button10 = createImageLabelBox ("pic/capture.png", "截屏");
    label_button11 = createImageLabelBox ("pic/audio.png", "语音");
    label_button12 = createImageLabelBox ("pic/video.png", "视频");
    
/*设置按钮图片*/
    gtk_container_add (GTK_CONTAINER (button1), label_button1);
    gtk_container_add (GTK_CONTAINER (button2), label_button2);
    gtk_container_add (GTK_CONTAINER (button3), label_button3);
    gtk_container_add (GTK_CONTAINER (button4), label_button4);
    gtk_container_add (GTK_CONTAINER (button5), label_button5);
    gtk_container_add (GTK_CONTAINER (button8), label_button8);
    gtk_container_add (GTK_CONTAINER (button9), label_button9);
    gtk_container_add (GTK_CONTAINER (button10), label_button10);
    gtk_container_add (GTK_CONTAINER (button11), label_button11);
    gtk_container_add (GTK_CONTAINER (button12), label_button12);
   
//设置发送和取消按钮
  button6 = gtk_button_new_with_label("取消(C)");
  button7 = gtk_button_new_with_label("发送(S)");

  showTextView= gtk_text_view_new();//textView1;
  inputTextView= gtk_text_view_new();//textView2;
  gtk_text_view_set_editable(GTK_TEXT_VIEW(showTextView),FALSE);
  printf("------------!7\n");
  
  gtk_box_pack_start(GTK_BOX(vbox4),scrolled_window1,TRUE,TRUE,0);
  gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolled_window1), showTextView);

  gtk_box_pack_start(GTK_BOX(vbox5),scrolled_window2,TRUE,TRUE,0);
  gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolled_window2), inputTextView);

  gag = gtk_accel_group_new();
  gtk_widget_add_accelerator(button7,"clicked",gag,GDK_s,GDK_CONTROL_MASK,GTK_ACCEL_VISIBLE);
  gtk_widget_add_accelerator(button6,"clicked",gag,GDK_c,GDK_CONTROL_MASK,GTK_ACCEL_VISIBLE);
  gtk_window_add_accel_group(GTK_WINDOW(mainWindow),gag);
  printf("------------!8\n");


  gtk_widget_set_size_request(hbox1,540,40);
  gtk_widget_set_size_request(hbox3,340,35);
  gtk_widget_set_size_request(vbox5,340,90);
  gtk_widget_set_size_request(vbox4,340,265);
  gtk_widget_set_size_request(hbox4,340,30);
  gtk_widget_set_size_request(vbox6,180,200);
  gtk_widget_set_size_request(vbox7,180,235);


  printf("------------!9\n");
  gtk_box_pack_start(GTK_BOX(vbox1),hbox1,FALSE,FALSE,0);
  gtk_box_pack_start(GTK_BOX(vbox1),hbox2,FALSE,FALSE,0);

  gtk_box_pack_start(GTK_BOX(hbox2),vbox2,FALSE,FALSE,0);
  gtk_box_pack_start(GTK_BOX(hbox2),vbox3,FALSE,FALSE,0);

  gtk_box_pack_start(GTK_BOX(vbox2),vbox4,TRUE,TRUE,0);
  gtk_box_pack_start(GTK_BOX(vbox2),hbox3,TRUE,TRUE,0);
  gtk_box_pack_start(GTK_BOX(vbox2),vbox5,TRUE,TRUE,0);
  gtk_box_pack_start(GTK_BOX(vbox2),hbox4,FALSE,FALSE,0);

  gtk_box_pack_start(GTK_BOX(vbox3),vbox6,TRUE,TRUE,0);
  gtk_box_pack_start(GTK_BOX(vbox3),vbox7,TRUE,TRUE,0);

  gtk_box_pack_start(GTK_BOX(hbox1),button1,FALSE,FALSE,0);
  gtk_box_pack_start(GTK_BOX(hbox1),button2,FALSE,FALSE,0); 
  gtk_box_pack_start(GTK_BOX(hbox1),button10,FALSE,FALSE,0);
  gtk_box_pack_start(GTK_BOX(hbox1),button3,FALSE,FALSE,0);
  gtk_box_pack_start(GTK_BOX(hbox1),button11,FALSE,FALSE,0);
  gtk_box_pack_start(GTK_BOX(hbox1),button12,FALSE,FALSE,0);

 
  

  gtk_box_pack_start(GTK_BOX(hbox3),button4,FALSE,FALSE,0);
  gtk_box_pack_start(GTK_BOX(hbox3),button5,FALSE,FALSE,0);
  gtk_box_pack_start(GTK_BOX(hbox3),button8,FALSE,FALSE,0);
  gtk_box_pack_start(GTK_BOX(hbox3),button9,FALSE,FALSE,0);
  


  gtk_box_pack_end(GTK_BOX(hbox4),button7,FALSE,FALSE,0);
  gtk_box_pack_end(GTK_BOX(hbox4),button6,FALSE,FALSE,0);
  
  
  gtk_box_pack_start(GTK_BOX(vbox7),selfLabel,FALSE,FALSE,20);
  gtk_box_pack_start(GTK_BOX(vbox7),buttonMyFace,FALSE,FALSE,10);
  gtk_box_pack_start(GTK_BOX(vbox6),label_ip,FALSE,FALSE,0);
  gtk_box_pack_start(GTK_BOX(vbox6),label_name,FALSE,FALSE,0);
  gtk_box_pack_start(GTK_BOX(vbox6),otherLabel,FALSE,FALSE,0);
  gtk_box_pack_start(GTK_BOX(vbox6),buttonHiFace,FALSE,FALSE,0);
/*设置按钮属性为不可见*/
  gtk_button_set_relief(GTK_BUTTON(button1),GTK_RELIEF_NONE);
gtk_button_set_relief(GTK_BUTTON(button2),GTK_RELIEF_NONE);
gtk_button_set_relief(GTK_BUTTON(button3),GTK_RELIEF_NONE);
gtk_button_set_relief(GTK_BUTTON(button4),GTK_RELIEF_NONE);
gtk_button_set_relief(GTK_BUTTON(button5),GTK_RELIEF_NONE);
gtk_button_set_relief(GTK_BUTTON(button6),GTK_RELIEF_NONE);
gtk_button_set_relief(GTK_BUTTON(button7),GTK_RELIEF_NONE);
gtk_button_set_relief(GTK_BUTTON(button8),GTK_RELIEF_NONE);
gtk_button_set_relief(GTK_BUTTON(button9),GTK_RELIEF_NONE);
gtk_button_set_relief(GTK_BUTTON(button10),GTK_RELIEF_NONE);
gtk_button_set_relief(GTK_BUTTON(button11),GTK_RELIEF_NONE);
gtk_button_set_relief(GTK_BUTTON(button12),GTK_RELIEF_NONE);
gtk_button_set_relief(GTK_BUTTON(buttonMyFace),GTK_RELIEF_NONE);
gtk_button_set_relief(GTK_BUTTON(buttonHiFace),GTK_RELIEF_NONE);
  /* Finally we add hbox to the window, and show all widgets */
 // gtk_container_add(GTK_CONTAINER(aspect_frame),vbox1);
  printf("------------!10\n");
  gtk_container_add(GTK_CONTAINER(mainWindow),vbox1);
 // gtk_container_add(GTK_CONTAINER(window), vbox1);
 printf("------------!10.5\n");
   /* 连接换肤回调函数 */
  g_signal_connect(button2,"clicked",changeFace,mainWindow);
/* 连接字体回调函数 */
  g_signal_connect(button4,"clicked",fontSelect,ip);
 /* 连接添加表情回调函数 */
  g_signal_connect(button5,"clicked",addFace,ip);
/*连接形象更换回调函数*/
 g_signal_connect (G_OBJECT (button1), "clicked",G_CALLBACK (changePersonFace),NULL);
  /* 连接文件选择回调函数 */
 g_signal_connect (G_OBJECT (button3), "clicked",G_CALLBACK (fileSelect),ip);
  g_signal_connect(button6,"clicked",undo,ip);
  g_signal_connect(button7,"clicked",sendEchoToTextView,ip);
/* 日历 */
  g_signal_connect(button8,"clicked",createCalendar,ip);
 /*记录*/
  g_signal_connect(button9,"clicked",createChatRecordWindow,ip);
/*截屏*/
  g_signal_connect(button10,"clicked",capture,ip);
  GtkTextBuffer *buffer = gtk_text_view_get_buffer(showTextView);
  gtk_text_buffer_get_end_iter(buffer,&iter);
  gtk_text_buffer_create_tag(buffer,"blue_foreground","foreground","blue",NULL);
  gtk_text_buffer_create_tag(buffer,"green_foreground","foreground","green",NULL);
  gtk_text_buffer_create_tag(buffer,"red_foreground","foreground","red",NULL);
  g_print("show is:%x\n",showTextView);
  g_print("input is:%x\n",inputTextView);

/*对聊天窗口结构体进行赋值*/
  chat.mainWindow = mainWindow;
  chat.showTextView = showTextView;
  chat.inputTextView = inputTextView;
  chat.showWindow = scrolled_window1;
  chat.inputWindow = scrolled_window2;
  chat.selfImage = selfImage;
  chat.otherImage = otherImage;
  chat.selfLabel = selfLabel;
  chat.otherLabel = otherLabel;
  chat.from = addr;
/*将结构体加入队列*/
  addChat(&chat);
  if(mode)
      gtk_widget_show_all(mainWindow);
  printf("------------!11\n");
  if(file_thread()==-1)printf("create file rec thread  faile");;
  printf("------------!12\n");
}  
static void getStrTime(gchar *buf,guint n){
    time_t ctime;
    size_t size;
    struct tm *p;
    time(&ctime);
    p = localtime(&ctime);
    strftime(buf,n,"  %Y-%m-%d %H:%M:%S",p);
    size = strlen(buf);
    buf[size++] = '\n';
    buf[size] = '\0';
    return ;
}

/**************************************************/
/*函数名：changePersonFace
/*功能：更换人物形象
/*时间：
/*参数：void
/*返回值：void
/*作者：
/***************************************************/
void changePersonFace(void* argv)
{
	GtkWidget *window;	
	GtkWidget *button;	
	GtkWidget *vbox;
	GtkWidget *hbox;	GtkWidget *list;
        GtkWidget *view;
	GtkTreeModel *model;
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
	GtkTreeSelection *selection; 
	window=gtk_window_new(GTK_WINDOW_TOPLEVEL);	
	gtk_window_set_default_size(GTK_WINDOW(window),200,200);
	g_signal_connect(G_OBJECT(window),"delete_event",G_CALLBACK(gtk_main_quit),NULL);
	gtk_window_set_title(GTK_WINDOW(window),"列表控件");	
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	gtk_container_set_border_width(GTK_CONTAINER(window),10);

	/*********添加列表显示内容**********/
        GtkListStore *list_store;
	GtkTreeIter iter;
	gint i;
	GdkPixbuf *pixbuf;
	list_store  = gtk_list_store_new(N_COLUMN,GDK_TYPE_PIXBUF,G_TYPE_STRING); 

	for(i=0;i<2;i++)
	{
		pixbuf = gdk_pixbuf_new_from_file(image[i].fcimage, NULL);		
		gtk_list_store_append(list_store,&iter);
		gtk_list_store_set(list_store,&iter,PIXBUF_COLUMN,pixbuf,TEXT_COLUMN,image[i].fcname,-1);
	}

        model = GTK_TREE_MODEL(list_store);
	view = gtk_tree_view_new_with_model(model);
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(view), FALSE);
	renderer = gtk_cell_renderer_pixbuf_new();
	 column = gtk_tree_view_column_new_with_attributes(
                     "Icon", renderer, 
                     "pixbuf", PIXBUF_COLUMN,
                      NULL);
   	 gtk_tree_view_append_column(GTK_TREE_VIEW (view), column);

	//column = gtk_tree_view_column_new_with_attributes("shuzi",renderer,"text",ID_COLUMN,NULL);
	//gtk_tree_view_append_column(GTK_TREE_VIEW(view),column);

	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("wenben",renderer,"text",TEXT_COLUMN,NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(view),column);
	//创建纵向盒子
	vbox=gtk_vbox_new(FALSE,0);	
	gtk_container_add(GTK_CONTAINER(window),vbox);
        
	//创建观察框
	//视区控件GtkViewPort,它也是一个容器(只能容纳一个控件)，在功能上和框架大致相同,主要是美化控件外观的作用
	//两个参数用来描述可调整的边界的值，默认时用NULL代替
	//view = gtk_viewport_new(NULL,NULL);	
	gtk_box_pack_start(GTK_BOX(vbox),view,TRUE,TRUE,5);


	//创建横向的盒子
	hbox = gtk_hbox_new(FALSE,0);
	gtk_box_pack_start(GTK_BOX(vbox),hbox,FALSE,FALSE,5);

	//创建标签
	button = gtk_button_new_with_label( "确定");
	
	gtk_box_pack_start(GTK_BOX(hbox),button,FALSE,FALSE,5);
	gtk_signal_connect(G_OBJECT(button),"clicked",widget_destroy,window);	
	button = gtk_button_new_with_label("返回");
	
	gtk_box_pack_start(GTK_BOX(hbox),button,FALSE,FALSE,5);
	gtk_signal_connect(G_OBJECT(button),"clicked",widget_destroy,window);

	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(view));
	g_signal_connect(selection, "changed", G_CALLBACK(on_select),NULL);

	gtk_widget_show_all(window);
        
	gtk_main();
}

/**************************************************/
/*函数名：on_select
/*功能：选择更换的形象
/*时间：
/*参数：
         GtkWidget* widget :控件指针
/*返回值：void
/*作者：
/***************************************************/
void  on_select(GtkWidget *widget){
	printf("*********************start************\n");
	GtkTreeIter iter;
	GtkTreeModel *model;
	GtkWidget *img ;
	char *value;
	int i;
	if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(widget), &model, &iter)) 
	{
		printf("*********************gtk_tree_model************\n");
		gtk_tree_model_get(model, &iter,TEXT_COLUMN, &value,  -1);
		for(i=0;i<2;i++){
		printf("******this is loop %d ***\n",i);
			if(NULL == strcmp(image[i].fcname,value)){
				printf("the img %d\n",i);
				img = gtk_image_new_from_file(image[i].fcimage);
				printf("img bass !\n");				
				gtk_button_set_image(GTK_BUTTON(buttonMyFace),img);
				printf("the button face %d\n",i);
			}
		}
		printf("%s\n",value);
		g_free(value);
	}
}

/**************************************************/
/*函数名：widget_destroy
/*功能：销毁窗体
/*时间：
/*参数：
         GtkWidget* widget :控件指针
	 GtkWidget* window :窗口控件
/*返回值：void
/*作者：
/***************************************************/
void widget_destroy(GtkWidget *widget,GtkWidget *window){
	gtk_widget_destroy(window);
}

static int file_thread(void)
{
  int ret =0;
  pthread_t file_rev ;
 /*创建一条文件传输监听线程*/
  if((ret = pthread_create(&file_rev,NULL,(void*)file_recv,NULL)) != 0)
  {
		printf("thread create faile !");	
		return -1 ;
  }		
}

