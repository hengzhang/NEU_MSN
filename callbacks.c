/********************************************/
/*名称：callbacks.c
/*描述： 1、该文件定义了程序中通用的功能函数
	2、定义了主界面的回调函数
		a.将整形数值转换成图片文件名---getPicname
		b.将图片转化为GdkPixbuf-----create_pixbuf
		c.制作带图片的按钮-----------pic_button
		d.刷新按钮的回调函数---------lin_refresh
		e.ip输入的回调函数-----------enter_ip
		f.显示帮助信息--------------show_about
		g.关闭信息提示回调函数--------show_question
		h.显示任务栏菜单-------------trayView
		i.显示任务栏图标-------------trayIconActivated
		j.弹出任务栏菜单-------------trayIconPopup
/*作者：张珩
/*日期：2010－07-01
/********************************************/
#include "i_main_callbacks.h"
#include "get_vbox_mid.h"
#include "creat_main.h"
#include "msg_list.h"
#include "m_user.h"
#include <assert.h>
extern struct Msg *Msghead;
/*********************************************/
//根据指定的数字编码生成jpg文件名称
/*********************************************/
char *getPicname(int num)
{
	static char c[8];
	char *p = ".jpg";
	if(num<10)
	{
		c[0] = num + 48;
		c[1]= '\0';

	}
	else{
		c[0] = num/10+48;
		c[1] = num%10+48;
		c[2]= '\0';
	}
	strcat(c,p);//warning
	return c;
}
/*********************************************/
//根据指定的文件名称生成GdkPixbuf
/*********************************************/
GdkPixbuf *create_pixbuf(const gchar * filename)
{
	GdkPixbuf *pixbuf;
	GError *error = NULL;
	pixbuf = gdk_pixbuf_new_from_file(filename, &error);
	if(!pixbuf) {
	fprintf(stderr, "%s\n", error->message);
	g_error_free(error);
	}
	return pixbuf;
}

/*********************************************/
//根据指定的文件名称生成Button
/*********************************************/
GtkWidget *pic_button( gchar *filename)
{
	GtkWidget *image;
	GtkWidget *button;
	button = gtk_button_new ();
	image = gtk_image_new_from_file (filename);	
	gtk_container_add (GTK_CONTAINER (button), image);
	gtk_container_set_border_width (GTK_CONTAINER (button), 0);
	return button;
}

/*********************************************/
//实时刷新界面
/*********************************************/
void lin_refresh(void)
{
  	
    GtkTreeStore *treestore;
    GtkTreeIter toplevel, child;
	remove_all();
	treestore = GTK_TREE_STORE(gtk_tree_view_get_model(
       		GTK_TREE_VIEW(view)));
    gchar *p = NULL;
    gchar *q = NULL;
    struct sockaddr_in addr;
    int i,j;
    GList *group = NULL;
    GList *user = NULL;
    LinpopUser *temp ;
    char buf[256];
    gboolean result;
    //userListForeach(printUser,NULL);
    for(group = getGroupList();group != NULL;group = group->next){
        p = (gchar *)group->data;
	gtk_tree_store_append(treestore, &toplevel, NULL);
	gtk_tree_store_set(treestore, &toplevel,COLUMN, p,-1);
        g_print("group:%s\n",p);
	for(user = getGroupUserList(p);user != NULL;user = user->next){
	    /*    q = (gchar *)user->data;
		gtk_tree_store_append(treestore, &child, &toplevel);
		gtk_tree_store_set(treestore, &child,COLUMN,q,-1); 
                g_print("user:%s\n",q);*/
          bzero(buf,sizeof(buf));
	        q = (gchar *)user->data;
         temp = g_malloc(sizeof(*temp));
        inet_aton(q,&addr.sin_addr);
        result = getUserByIP(addr,temp);
        assert(result == TRUE);
            strcat(buf,temp->user);
            strcat(buf,"(");
            strcat(buf,q);
            strcat(buf,")");
            strcpy(q,buf);
            g_print("q is:%s\n",q);
		    gtk_tree_store_append(treestore, &child, &toplevel);
		    gtk_tree_store_set(treestore, &child,COLUMN,q,-1); 
            g_print("user:%s\n",q);
            freeUser(temp);
	}
    }
 
    return GTK_TREE_MODEL(treestore);

}
/*********************************************/
//IP查找输入处理
/*********************************************/
void enter_ip( GtkWidget *widget,GtkWidget *entry )
{
	gchar *userip;
	//gtk_editable_delete_text(GTK_EDITABLE(entry),0,3);
	userip = gtk_entry_get_text (GTK_ENTRY (entry));
    struct
}

/*********************************************/
//帮助信息
/*********************************************/

/*********************************************/
//关闭信息提示
/*********************************************/
void show_question(GtkWidget *widget, gpointer window)
{
	GtkWidget *dialog;
	dialog = gtk_message_dialog_new(window,
	    GTK_DIALOG_DESTROY_WITH_PARENT,
	    GTK_MESSAGE_QUESTION,
	    GTK_BUTTONS_YES_NO,
	    "确认退出程序?");
	gtk_window_set_title(GTK_WINDOW(dialog), "Question");
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}


/*********************************************/
//任务栏图标－显示主界面
/*********************************************/
void trayView(GtkMenuItem *item, gpointer window)
{
	gtk_widget_show(GTK_WIDGET(window));
	gtk_window_deiconify(GTK_WINDOW(window));   
}
/*********************************************/
//任务栏图标－单击
/*********************************************/
void trayIconActivated(GObject *trayIcon, gpointer window)
{
	if(gtk_status_icon_get_blinking(GTK_STATUS_ICON (trayIcon))==FALSE){
		gtk_widget_show(GTK_WIDGET(window));
		gtk_window_deiconify(GTK_WINDOW(window));
	}
	else{
printf("-------------------[click to create window start]----------------\n");
		//if(Msghead != NULL)
		char ip[20]; 
		strcpy(ip,Msghead->ip);
printf("ip:%s\n",ip);
		char name[20]; 
		strcpy(name,Msghead->name);
printf("name:%s\n",name);
		char msg[1024];
		bzero(msg,1024);
		getMsg(ip,msg);
printf("msg:%s\n",msg);
		if(Msghead == NULL)
			gtk_status_icon_set_blinking (trayIcon,FALSE);
		
		createChatWindow(ip,name,msg);
		//showMessage(msg.message,p->textViewAll);

printf("-------------------[click to create window end ]----------------\n");
	}

}
/*********************************************/
//任务栏图标－弹出菜单
/*********************************************/
void trayIconPopup(GtkStatusIcon *status_icon, guint button, guint32 activate_time, gpointer popUpMenu)
{
	gtk_menu_popup(GTK_MENU(popUpMenu), NULL, NULL, gtk_status_icon_position_menu, status_icon, button, activate_time);
}
/*********************************************/
//任务栏图标
/*********************************************/
gboolean window_state_event (GtkWidget *widget, GdkEventWindowState *event, gpointer data)
{
	if(event->changed_mask == GDK_WINDOW_STATE_ICONIFIED && (event->new_window_state == 						GDK_WINDOW_STATE_ICONIFIED || event->new_window_state == 			(GDK_WINDOW_STATE_ICONIFIED | GDK_WINDOW_STATE_MAXIMIZED)))
	{
		gtk_widget_hide (GTK_WIDGET(widget));
		gtk_status_icon_set_visible(GTK_STATUS_ICON(trayIcon), TRUE);
	}
	else if(event->changed_mask == GDK_WINDOW_STATE_WITHDRAWN && (event->new_window_state == 						GDK_WINDOW_STATE_ICONIFIED || event->new_window_state == 						(GDK_WINDOW_STATE_ICONIFIED | GDK_WINDOW_STATE_MAXIMIZED)))
	{
		gtk_status_icon_set_visible(GTK_STATUS_ICON(trayIcon), TRUE);
	}
	return TRUE;
}
