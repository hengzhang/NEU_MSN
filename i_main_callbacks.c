/********************************************/
/*名称：i_main_callbacks.c
/*描述： 1、该文件定义了程序中通用的功能函数
	2、定义了主界面的回调函数
/*作者：
/*日期：
/********************************************/
#ifndef I_MAIN_CALLBACKS_
#define I_MAIN_CALLBACKS_
#include <netinet/in.h>
#include <assert.h>
#include "i_main_callbacks.h"
#include "i_main_window.h"
#include "i_chat_window.h"
#include "m_user.h"
/*******************************************************************/
/*
函数名:		void onChanged(GtkWidget *widget, gpointer entry);
参数:		widget: 控件指针,  entry : 回调函数调用参数
返回值:          空
函数描述:    改变树形菜单列表改变
*/
static int doWindowShow(LinpopChat *chat,void *arg);
void onChanged(GtkWidget *widget, gpointer entry)
{
  GtkTreeIter iter;
  GtkTreeModel *model;
  char *value;
  if (gtk_tree_selection_get_selected(
      GTK_TREE_SELECTION(widget), &model, &iter))
	{
    	gtk_tree_model_get(model, &iter,COLUMN, &value,  -1);
    	//gtk_entry_set_text(GTK_ENTRY(entry), value);
        strcpy(selectText,value);
    	g_free(value);
  	}
}
/*******************************************************************/
/*
函数名:		void listRefresh(void);
参数:		空
函数描述:	刷新好友列表
返回值:          空
/*******************************************************************/
void listRefresh(void)
{
    GtkTreeStore *treestore;
    GtkTreeIter toplevel, child;
    remove_all();
    treestore = GTK_TREE_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(view)));
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
    //我的好友列表建立
    p="特别好友";
    gtk_tree_store_append(treestore, &toplevel, NULL);
    gtk_tree_store_set(treestore, &toplevel,COLUMN, p,-1);
    for(user = getGroupUserList(p);user != NULL;user = user->next){
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
            g_print("q is special:%s\n",q);
		    gtk_tree_store_append(treestore, &child, &toplevel);
		    gtk_tree_store_set(treestore, &child,COLUMN,q,-1);
            g_print("user special :%s\n",q);
            freeUser(temp);
	}
    //gtk_tree_store_append(treestore, &child, &toplevel);
    //q=NULL;
    //gtk_tree_store_set(treestore, &child,COLUMN,q,-1);
    //!!!!!!!!!!!!
    for(group = getGroupList();group != NULL;group = group->next){
        p = (gchar *)group->data;
        if((strcmp(p,"黑名单")== 0) || (strcmp(p,"特别好友")== 0)){
              continue;
	      g_print("ignore\n");
	}
	gtk_tree_store_append(treestore, &toplevel, NULL);
	gtk_tree_store_set(treestore, &toplevel,COLUMN, p,-1);
        g_print("n group:%s\n",p);
	for(user = getGroupUserList(p);user != NULL;user = user->next){
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
            g_print("n q is:%s\n",q);
		    gtk_tree_store_append(treestore, &child, &toplevel);
		    gtk_tree_store_set(treestore, &child,COLUMN,q,-1);
            g_print("n user:%s\n",q);
            freeUser(temp);
	}
    }
    //黑名单列表建立
     p="黑名单";
    gtk_tree_store_append(treestore, &toplevel, NULL);
    gtk_tree_store_set(treestore, &toplevel,COLUMN, p,-1);
    for(user = getGroupUserList(p);user != NULL;user = user->next){
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
            g_print("b q is:%s\n",q);
		    gtk_tree_store_append(treestore, &child, &toplevel);
		    gtk_tree_store_set(treestore, &child,COLUMN,q,-1);
            g_print("b user:%s\n",q);
	    g_print("diff\n");
            freeUser(temp);
	}
   // gtk_tree_store_append(treestore, &child, &toplevel);
    //q=NULL;
    //gtk_tree_store_set(treestore, &child,COLUMN,q,-1);
    //!!!!!!!!!!!!
	numToStr(buf,getUserNumber());
	strcat(buf," friends online");
	gtk_entry_set_text (GTK_ENTRY (entry), buf);
    /*for(i = 0,p = g_list_nth_data(group,i); p != NULL ;i++){
	gtk_tree_store_append(treestore, &toplevel, NULL);
	gtk_tree_store_set(treestore, &toplevel,COLUMN, p,-1);
        g_print("p:%s\n",p);
	user = getGroupUserList(p);
	for(j = 0,q = g_list_nth_data(user,j); q != NULL;j++ ){
		gtk_tree_store_append(treestore, &child, &toplevel);
		gtk_tree_store_set(treestore, &child,COLUMN,q,-1);
                g_print("q:%s\n",q);
	}
    }*/
}
gboolean closeTimeoutWindow(GtkWidget *widget){
    gtk_widget_destroy(widget);
    return FALSE;
}
/*******************************************************************/
/*
函数名:		void startChat(GtkTreeView *treeView,GtkTreePath *path);
参数:		treeView: 传入的树形视图,  path : 得到树形的路径
返回值:          空
函数描述:    调用对话框
*/
void startChat(GtkTreeView *treeView,GtkTreePath *path){
    GtkTreeModel *model;
    GtkTreeIter iter;
    GtkTreeSelection *selection;
    char *value;
    int i;
    struct sockaddr_in addr ;
    LinpopUser *temp = g_malloc(sizeof(*temp));
    gboolean result;
    char ip[16];
    g_print("path is:%x\n",path);
    model = gtk_tree_view_get_model(treeView);
    selection = gtk_tree_view_get_selection(treeView);
    if(gtk_tree_model_get_iter(model,&iter,path)){
        if(gtk_tree_selection_get_selected(selection,&model,&iter)){
	    gtk_tree_model_get(model,&iter,COLUMN,&value,-1);
	    g_print("value is:%s\n",value);
	    bzero(ip,sizeof(ip));
	    i = strlen(value);
	    for(;(*value != '(') && i;value++,i--);
	    if(!i){
	        g_print("invalid!\n");
	        return ;
	    }
	    g_print("str2:%s\n",value);
	    for(++value,i = 0;*value != ')';value++,i++)
		ip[i] = *value;
	    g_print("ip:%s\n",ip);
	    inet_aton(ip,&addr.sin_addr);
	    result = getUserByIP(addr,temp);
	    g_print("%d",result);
	    if(result == FALSE){
		g_print("No user at %s,cannot create chat window",ip);
		g_free(temp);
		return ;
	    }
	    createChatWindow(inet_ntoa(temp->from.sin_addr),temp->user,SHOW);
	    freeUser(temp);
	}
    }
}
/*******************************************************************/
/*
函数名:		void searchFriend( GtkWidget *widget,GtkWidget *entry );
参数:		widget ： 用户查找的控件 ，  entry ： 用户查找的IP 文本输入框
函数描述:    查找好友的回调函数
返回值:          空
*/
void searchFriend( GtkWidget *widget,GtkWidget *search)
{
	gchar *userip;
	//gtk_editable_delete_text(GTK_EDITABLE(entry),0,3);
    int result;
    gchar buf[128];
    //g_print("search OK,entry is:%x\n",entry);
	userip = gtk_entry_get_text (GTK_ENTRY (entry));
    g_print("user ip is:%s\n",userip);
    struct sockaddr_in addr;
    bzero(&addr,sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(LINPOP_DEFAULT_PORT);
    result = inet_aton(userip,&addr.sin_addr);
    if(!result){
        g_print("Not find\n");
    }else{
        //result = appendBroadcastList(userip);
	g_print("result is:%d\n",result);
	g_print("result is:%d\n",result);
        LinpopSend(&addr,LINPOP_BR_EXIT,LinpopGetNickWithAbsence());
        LinpopSend(&addr,LINPOP_BR_ENTRY,LinpopGetNickWithAbsence());
        //LinpopSendBroadcast(LINPOP_BR_EXIT,"NEU_MSG");
        //LinpopSendBroadcast(LINPOP_BR_ENTRY,"NEU_MSG");
    }
	numToStr(buf,getUserNumber());
	strcat(buf," friends online");
	gtk_entry_set_text (GTK_ENTRY (entry), buf);
}
/*******************************************************************/
/*
函数名:		void clearEntry(GtkWidget *widget,GtkWidget *search);
参数:		widget ： 用户查找的控件 ，  entry ： 用户查找的IP 文本输入框
函数描述:    清空用户查找框
返回值:          空
*/
void clearEntry(GtkWidget *widget,GtkWidget *search){
    gtk_entry_set_text(entry,"");
    return ;
}
/*******************************************************************/
/*
函数名:		void show_about(void);
参数:		空
函数描述:	软件帮助信息
返回值:          空
*/
void show_about(void)
{
	const gchar const *rc_authors[] = {"钟华刚",NULL,NULL};
	const gchar const *rc_documenters[] = {"张珩",NULL,NULL};
	const gchar const *rc_artists[] = {"董俊杰",NULL,NULL};
	GtkWidget *dialog = gtk_about_dialog_new();
	gtk_about_dialog_set_name(GTK_ABOUT_DIALOG(dialog), "NEU_MSN");
	gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(dialog), "NEU_MSN");
	gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog),"V-2.0");
	gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog),"copyright@NEU team 6");
	gtk_about_dialog_set_license(GTK_ABOUT_DIALOG(dialog),"NEU_MSN是自由软件；\n您可以重新发布它;\nNEU_MSN是一款局域网聊天软件，\n提供基于Internet标准的信息收发功能; \nNEU_MSN V-2.0正式版致力于为您提供更为便捷舒适的NEU_MSN使用体验;\n本软件完全免费，欢迎您使用");
	gtk_about_dialog_set_authors(GTK_ABOUT_DIALOG(dialog),(const gchar **)rc_authors);
	gtk_about_dialog_set_artists(GTK_ABOUT_DIALOG(dialog),(const gchar **)rc_documenters);
	gtk_about_dialog_set_documenters(GTK_ABOUT_DIALOG(dialog),(const gchar **)rc_artists);
	gtk_about_dialog_set_translator_credits(GTK_ABOUT_DIALOG(dialog),"ANGRY BIRD");
	gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog), "NEU_MSN是一个局域网聊天软件.");
	gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(dialog), "http://www.neusoft.com");
	gtk_dialog_run(GTK_DIALOG (dialog));
	gtk_widget_destroy(dialog);
}
/*******************************************************************/
/*
函数名:		void trayView(GtkMenuItem *item, gpointer window);
参数:		item:菜单项指针；window：程序主窗体指针
函数描述:	回调函数，显示主窗体
返回值:          空
*/
void trayView(GtkMenuItem *item, gpointer window)
{
	gtk_widget_show(GTK_WIDGET(window));
	gtk_window_deiconify(GTK_WINDOW(window));
}
/*******************************************************************/
/*
函数名:		void trayIconPopup(GtkStatusIcon *status_icon, guint button, guint32 activate_time, gpointer popUpMenu);
参数:		status_icon:任务栏图标指针；popUpMenu：弹出菜单指针
函数描述:	回调函数-右键单机任务栏图标，弹出选择菜单。
返回值:          空
*/
void trayIconPopup(GtkStatusIcon *status_icon, guint button, guint32 activate_time, gpointer popUpMenu)
{
	gtk_menu_popup(GTK_MENU(popUpMenu), NULL, NULL, gtk_status_icon_position_menu, status_icon, button, activate_time);
}
/*******************************************************************/
/*
函数名:		void show_question(GtkWidget *widget, gpointer window);
参数:		widget:控件指针；window：主窗体指针
函数描述:	用户关闭程序时提示信息
返回值:          空
*/
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
/*******************************************************************/
/*
函数名:		gboolean window_state_event (GtkWidget *widget, GdkEventWindowState *event, gpointer trayIcon);
返回值:          布尔值
*/
gboolean window_state_event (GtkWidget *widget, GdkEventWindowState *event, gpointer data)
{
	if(event->changed_mask == GDK_WINDOW_STATE_ICONIFIED && (event->new_window_state == GDK_WINDOW_STATE_ICONIFIED || event->new_window_state == (GDK_WINDOW_STATE_ICONIFIED | GDK_WINDOW_STATE_MAXIMIZED)))
	{
		gtk_widget_hide (GTK_WIDGET(widget));
		gtk_status_icon_set_visible(GTK_STATUS_ICON(trayIcon), TRUE);
	}
	else if(event->changed_mask == GDK_WINDOW_STATE_WITHDRAWN && (event->new_window_state == GDK_WINDOW_STATE_ICONIFIED || event->new_window_state == 						(GDK_WINDOW_STATE_ICONIFIED | GDK_WINDOW_STATE_MAXIMIZED)))
	{
		gtk_status_icon_set_visible(GTK_STATUS_ICON(trayIcon), TRUE);
	}
	return TRUE;
}
void trayIconActivated(GObject *trayIcon, gpointer window)
{
	if(gtk_status_icon_get_blinking(GTK_STATUS_ICON (trayIcon))==FALSE){
		gtk_widget_show(GTK_WIDGET(window));
		gtk_window_deiconify(GTK_WINDOW(window));
	}else{
         chatListForeach((chatFunc)doWindowShow,NULL); 
         gtk_widget_show(GTK_WIDGET(window));
		 gtk_window_deiconify(GTK_WINDOW(window));
         gtk_status_icon_set_blinking(GTK_STATUS_ICON(trayIcon),FALSE);
    }
}
static int doWindowShow(LinpopChat *chat,void *arg){
    gtk_widget_show_all(chat->mainWindow);
    return 0;
}
#endif
