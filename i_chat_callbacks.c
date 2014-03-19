#ifndef __I_CHAT_CALLBACKS_
#define __I_CHAT_CALLBACKS_
#include <netinet/in.h>
#include <assert.h>
#include "i_chat.h"
#include "i_chat_window.h"
#include "m_user.h"
#include "g_properties.h"
#include "i_chat_callbacks.h"
//chat record interface
#include"i_rightResponse.h"
#include"i_list.h"
#include"i_textview.h"
#include"i_search_button.h"
#include"i_entry_IP.h"
#define MAX_MSG 2048

/**************************************************/
/*函数名：createChatRecordWindow
/*功能：创建聊天记录窗口
/*时间：
/*参数：
        GtkWidget *widget:控件指针
        gpointer ip:
/*返回值：void
/*作者：
/***************************************************/
void createChatRecordWindow(GtkWidget *widget,gpointer ip){
	GtkWidget *window;
	GtkWidget *vbox;
	GtkWidget *hbox_chatRecord;
	GtkWidget *scrolledwindow_person;
	GtkWidget *view;
	GtkWidget *personlist_menu_right;
	GtkWidget *personlist_menu_right_member;
//	GtkWidget *treeview_person;
	GtkWidget *list_person;
	GtkWidget *item;
	GtkWidget *scrolledwindow_text;
	GtkWidget *textview_chat;
	GtkWidget *hbox_search;
	GtkWidget *IP_label;
	GtkWidget *entry_IP;
	GtkWidget *button_search;
	GtkWidget *textview_menu_right;
	GtkWidget *textview_menu_right_member;

//初始化窗口
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (window), "聊天记录");
//纵向盒子，整体布局
  	vbox = gtk_vbox_new (FALSE, 0);
  	gtk_widget_show (vbox);
  	gtk_container_add (GTK_CONTAINER (window), vbox);

  	hbox_chatRecord = gtk_hbox_new (FALSE, 0);
  	gtk_widget_show (hbox_chatRecord);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox_chatRecord, TRUE, TRUE, 0);
//聊天成员滑动窗口
  	scrolledwindow_person = gtk_scrolled_window_new (NULL, NULL);
        gtk_widget_show (scrolledwindow_person);
  	gtk_box_pack_start (GTK_BOX (hbox_chatRecord), scrolledwindow_person, TRUE, TRUE, 0);
 	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolledwindow_person), GTK_SHADOW_IN);
//	gtk_widget_set_size_request (scrolledwindow_person, 300, 300);

// 	treeview_person = gtk_tree_view_new ();
//	gtk_widget_show (treeview_person);
//  	gtk_container_add (GTK_CONTAINER (scrolledwindow_person), treeview_person);
//	gtk_widget_set_size_request (treeview_person, 300, 300);

//创建观察框
	//视区控件GtkViewPort,它也是一个容器(只能容纳一个控件)，在功能上和框架大致相同,主要是美化控件外观的作用
	//两个参数用来描述可调整的边界的值，默认时用NULL代替
	view = gtk_viewport_new(NULL,NULL);
	gtk_container_add(GTK_CONTAINER(scrolledwindow_person),view);
//	gtk_box_pack_start(GTK_BOX(scrolledwindow_person),view,TRUE,TRUE,5);
	gtk_widget_set_size_request (view, 280, 420);
//创建列表
	list_person = creatList();
	gtk_container_add(GTK_CONTAINER(view),list_person);

	//初始化列表项顶，无选中及右键菜单
	item=gtk_list_item_new_with_label("IP          		");
	gtk_container_add(GTK_CONTAINER(list_person),item);
	gtk_widget_show(item);
//	list_add(NULL,NULL);
	int i;
	char* IP[3] = {{"10.10.31.1"},{"10.10.31.2"},{"10.10.31.3"}};
	for(i = 0; i < 3; i++)
	{
		list_add(NULL,IP[i]);
	}
        //设置列表框的选择模式(单选或多选)
	gtk_list_set_selection_mode(GTK_LIST(list_person),GTK_SELECTION_SINGLE);

//文本滑动窗口
  	scrolledwindow_text = gtk_scrolled_window_new (NULL, NULL);
  	gtk_widget_show (scrolledwindow_text);
  	gtk_box_pack_start (GTK_BOX (hbox_chatRecord), scrolledwindow_text, TRUE, TRUE, 0);
  	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolledwindow_text), GTK_SHADOW_IN);
//显示聊天文本域
  	textview_chat = creatTextView();
//  	gtk_widget_show (textview_chat);
  	gtk_container_add (GTK_CONTAINER (scrolledwindow_text), textview_chat);
//  	gtk_widget_set_size_request (textview_chat, 350, 420);
//	gtk_text_view_set_editable (GTK_TEXT_VIEW (textview_chat), FALSE);
//	gtk_text_buffer_set_text (gtk_text_view_get_buffer (GTK_TEXT_VIEW (textview_chat)),"此处显示文本", -1);
//横向的搜索区域
  	hbox_search = gtk_hbox_new (FALSE, 0);
  	gtk_widget_show (hbox_search);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox_search, TRUE, TRUE, 0);
//IP标签
	IP_label = gtk_label_new("用户IP:");
	gtk_box_pack_start (GTK_BOX (hbox_search), IP_label, FALSE, FALSE, 5);
//IP输入框
	entry_IP = creatEntry_IP();
 	gtk_box_pack_start (GTK_BOX (hbox_search), entry_IP, TRUE, TRUE, 0);
// 	gtk_entry_set_text (GTK_ENTRY (entry_IP), search_IP);
//	g_signal_connect(G_OBJECT(entry_IP),"toggle_overwrite",G_CALLBACK(list_add),NULL);
//  	gtk_entry_set_invisible_char (GTK_ENTRY (entry_IP), 9679);
//	search_IP = gtk_entry_get_text(entry_IP);		
//搜索按钮
	button_search = creat_search_button();
// 	button_search = gtk_button_new_with_mnemonic ("搜索");
//	gtk_widget_show (button_search);
//	g_signal_connect(G_OBJECT(button_search),"clicked",G_CALLBACK(list_add),NULL);
  	gtk_box_pack_start (GTK_BOX (hbox_search), button_search, FALSE, FALSE, 0);
//  	gtk_widget_set_size_request (button_search, 50, 50);
	gtk_widget_show_all(window);
    
}


/**************************************************/
/*函数名：closeChatWindow
/*功能：关闭聊天窗口
/*时间：
/*参数：
         GtkWidget *widget ：控件指针
         gpointer ip ：
/*返回值：void
/*作者：
/***************************************************/
void closeChatWindow(GtkWidget *widget,gpointer ip){
     struct sockaddr_in addr;
     LinpopChat chat;
     gboolean result;
     inet_aton(ip,&addr.sin_addr);
     result = getChatByIP(addr,&chat);
     assert(result == TRUE);
     deleteChatByIP(addr);
     g_free(ip);
     gtk_widget_destroy(widget);
     return ;
}

/**************************************************/
/*函数名：sendEchoToTextView
/*功能：在textView上显示返回的信息
/*时间：
/*参数：
         GtkWidget *widget ：控件指针
         gpointer ip ：
/*返回值：void
/*作者：
/***************************************************/
void sendEchoToTextView(GtkWidget *widget,gpointer ip){
     struct sockaddr_in addr;
     LinpopChat chat;
     gboolean result;
     LinpopUser *user ;
     size_t sizeText,sizeMsg;
     gchar *text = NULL;
     gchar *s,*t;
     inet_aton(ip,&addr.sin_addr);
     result = getChatByIP(addr,&chat);
     GtkTextBuffer *buffer = NULL;
     GtkTextIter start,end;
     assert(result == TRUE);
     buffer = gtk_text_view_get_buffer(chat.inputTextView);
     gtk_text_buffer_get_bounds(buffer,&start,&end);
     text = gtk_text_buffer_get_text(buffer,&start,&end,FALSE);
    
    for(sizeMsg = 0,t = chat.msg; *t;t++){
        if(*t == '\\'){
	    if(*(t+1) == '\\'){
	        sizeMsg++;
	    }
	    t++;
	}else{
	    sizeMsg++;
	}
    }
    for(s = text + sizeMsg;*s;s++,t++){
        if(*s == '\\'){
	    *t++ = '\\';
	    *t = '\\'; 
	}else{
	    *t = *s;
	}
    }
    *t = '\0';
         if(chat.msg[0] == '\0'){
	     printInfoToTextView(chat.showTextView,option.user,"Don`t send empty message!");
	     return ;
	  }
         user = g_malloc(sizeof(*user));
	 result = getUserByIP(addr,user);
	 assert(result == TRUE);
	 g_print("chat.msg is:%s\n",chat.msg);
	 printEchoToTextView(chat.showTextView,option.user,chat.msg);
	 LinpopTimeoutSend(user->from,chat.msg,LINPOP_SENDMSG);
	 chat.msg[0] = '\0';
	 clearTextView(chat.inputTextView);
	 freeUser(user);
     return ;
}

/**************************************************/
/*函数名：undo
/*功能：取消操作
/*时间：
/*参数：
        GtkWidget *button:控件指针
        gpointer ip:
/*返回值：void
/*作者：
/***************************************************/
void undo(GtkWidget *widget,gpointer ip){
     struct sockaddr_in addr;
     LinpopChat chat;
     gboolean result;
     gchar text[MAX_MSG];
     inet_aton(ip,&addr.sin_addr);
     result = getChatByIP(addr,&chat);
     assert(result == TRUE);
     getTextFromTextView(chat.inputTextView,text,sizeof(text) - 1);
     text[sizeof(text)] = '\0';
     g_print("undo text is:%s\n",text);
     chat.msg[0] = '\0';
     return ;
}

/**************************************************/
/*函数名：capture
/*功能：截取当前屏幕图像
/*时间：
/*参数：
        GtkWidget *button:控件指针
        gpointer ip:
/*返回值：void
/*作者：
/***************************************************/
void capture(GtkWidget *button, gpointer ip)
{
	 int cmd, sec;
	 char *file = NULL;
	 char message[1024];
	 time_t tt;
	 struct tm *tm_ptr = NULL;
	 struct sockaddr_in addr;
	 gint x, y;
	 GdkScreen* cur_screen = NULL;
	 GdkWindow* window = NULL;
	 GdkPixbuf* pixbuf_screenshot = NULL;
	 GdkRectangle rect;
	 GdkRectangle screen_rect;
	 LinpopChat chat;
	 gboolean result;
	 file = (char *)g_malloc(32);
	 inet_aton(ip,&addr.sin_addr);
	 result = getChatByIP(addr,&chat);
	 assert(result == TRUE);
	 time(&tt);
	 tm_ptr = localtime(&tt);
	 strftime(file, 32, "%Y%m%d%H%M%S", tm_ptr);
	 strcat(file, ".jpg");
	 g_printf("ready to capture.....\r\n");
	 if (cur_screen == NULL)
	 cur_screen = gdk_screen_get_default (); 
	 screen_rect.x = 0;
	 screen_rect.y = 0;
	 screen_rect.width = gdk_screen_get_width (cur_screen); //get screen width
	 screen_rect.height = gdk_screen_get_height (cur_screen); //get screen height
	 g_print("screen_rect: x=%d,y=%d,w=%d,h=%d\n",screen_rect.x,screen_rect.y,screen_rect.width,screen_rect.height);
	 window = gdk_screen_get_root_window (cur_screen); //get window by screen
	 //Window xwin = GDK_WINDOW_XID(window);  //get window id
	 //g_print("xwindow id is %x\n",xwin);
	 gdk_window_get_origin (window, &x, &y); //get origin point
	 rect.x = x;
	 rect.y = y;
	 gdk_drawable_get_size (GDK_DRAWABLE (window), &rect.width,&rect.height); //get drawable size
	 g_print("rect: x=%d,y=%d,w=%d,h=%d\n",rect.x,rect.y,rect.width,rect.height);
	 if (! gdk_rectangle_intersect (&rect, &screen_rect, &rect)) { //obtain the intersect area from rect and screen_rect
	 g_print("Capture failed!...\r\n");
	 printInfoToTextView(chat.showTextView,option.user,"Capture failed!..\n");
	 return;
	 }
	 g_print("capture rect: x=%d,y=%d,w=%d,h=%d\n",rect.x,rect.y,rect.width,rect.height);
	 pixbuf_screenshot = gdk_pixbuf_get_from_drawable (NULL, window,
	 NULL,rect.x - x, rect.y - y, 0, 0,
	 rect.width, rect.height); //get pixbuf from drawable widget
	 gdk_pixbuf_save (pixbuf_screenshot, file, "jpeg", NULL, "quality","100", NULL); //save pixbuf to jpeg format file
	 g_object_unref (pixbuf_screenshot);  //free pixbuf
	 g_print("Capture saved!!!\r\n");
	sprintf(message,"%s %s",file,"saved in current director.");
	printInfoToTextView(chat.showTextView,option.user,message);

}

/**************************************************/
/*函数名：changeFace
/*功能：更换聊天界面皮肤
/*时间：
/*参数：
         GtkWidget* button :按钮控件指针
         GtkButton* window :窗口指针
/*返回值：void
/*作者：
/***************************************************/
void changeFace(GtkButton* button,GtkButton* window)
{
	GtkWidget *dialog;
	GtkColorSelection *colorsel;
	gint response;
        GdkColor color;

	dialog = gtk_color_selection_dialog_new("Ñ¡Ôñ±³¾°ÑÕÉ«");
	gtk_window_set_transient_for(GTK_WINDOW(dialog),GTK_WINDOW(window));
	colorsel = GTK_COLOR_SELECTION(GTK_COLOR_SELECTION_DIALOG(dialog)->colorsel);
	gtk_color_selection_set_has_opacity_control(colorsel,TRUE);
	gtk_color_selection_set_has_palette(colorsel,TRUE);

	gtk_signal_connect(GTK_OBJECT(dialog),"destroy",GTK_SIGNAL_FUNC(gtk_widget_destroy),&dialog);
	gtk_color_selection_set_previous_color(colorsel,&color);
	gtk_color_selection_set_current_color(colorsel,&color);
	response = gtk_dialog_run(GTK_DIALOG(dialog));
	if (response == GTK_RESPONSE_OK)
	{	
            gtk_color_selection_get_current_color (colorsel, &color);
            gtk_widget_modify_bg (window, GTK_STATE_NORMAL, &color);
	    
	}	
      
	gtk_widget_destroy(dialog);
}
#endif
