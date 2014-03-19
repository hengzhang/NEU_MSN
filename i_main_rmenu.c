/********************************************/
/*名称：i_main_rmenu.c
/*描述： 1、该文件定义了程序中用于创建右键菜单的函数
	2、定义了相应的回调函数
/*作者：
/*日期：
/********************************************/
#include <netinet/in.h>
#include <assert.h>
#include "m_user.h"
#include "i_main_window.h"
#include "i_main_rmenu.h"
#include "i_chat_window.h"


static void createProfileWindow(gchar *user,gchar *group,gchar *ip,gchar *absence);
/****************************************************/
/*
函数名:		creat_main
参数：		void
返回值:	    GtkWidget ＊
功能：	    得到右键菜单选项 控件
*/
/**************************************************/
GtkWidget* getRightMenu(void)
{
	GtkWidget *menu_right,*menu_items_right;
	int i;
	char buf[128];
	menu_right = gtk_menu_new ();
	for (i = 0; i < 7; i++)
	{
		switch(i){
			case 0:
				sprintf (buf, "单人聊天");
				break;
			case 1:
				sprintf (buf, "群组聊天");
				break;
			case 2:
				sprintf (buf, "查看资料");
				break;
                        case 3:
				sprintf (buf, "移入黑名单");
				break;
			case 4:
				sprintf (buf, "移出黑名单");
				break;
                        case 5:
				sprintf (buf, "移入特别好友");
				break;
                        case 6:
				sprintf (buf, "移出特别好友");
				break;
			}
		menu_items_right = gtk_menu_item_new_with_label (buf);
		gtk_menu_shell_append (GTK_MENU_SHELL (menu_right), menu_items_right);
		g_signal_connect_swapped (G_OBJECT (menu_items_right), "activate",G_CALLBACK (switchForRMenu),(gpointer) g_strdup (buf));
		gtk_widget_show (menu_items_right);
	}
	return menu_right;
}

/****************************************************/
/*
函数名:		right_button_press
参数：		wid : 传入的 右键控件, event ：生成的事件
返回值:	    gint
功能：	       右键菜单回调函数
*/
/**************************************************/
static gint responseToRMenu(GtkWidget * wid, GdkEvent * event)
 {
   GdkEventButton *bevent;
   if (event->type == GDK_BUTTON_PRESS)
     {
       bevent = (GdkEventButton *) event;
       if (bevent->button == 3)
         {
           gtk_menu_popup (GTK_MENU (wid), NULL, NULL, NULL, NULL,
                           bevent->button, bevent->time);
           return TRUE;
         }
     }

   return FALSE;
 }
/****************************************************/
/*
函数名:		right_button_press
参数：		wid : 传入的 右键控件,menu : 调用 创建的右键menu
返回值:	   void
功能：	       为控件添加右键菜单
*/
/**************************************************/
void addMenuToPopup(GtkWidget * wid, GtkMenu * menu)
 {
   g_signal_connect_swapped (G_OBJECT (wid), "event",
                             G_CALLBACK (responseToRMenu), menu);
 }

//****************************************************/
/*
函数名:		right_response
参数：		string : 传入 右键menu部分 选择的选项的字符串
返回值:	   void
功能：	       右键菜单响应
*/
/**************************************************/
void switchForRMenu(gchar *string)
{
	char* a[7] = {"单人聊天","群组聊天","查看资料","移入黑名单","移出黑名单","移入特别好友","移出特别好友"};
	if(strcmp(string,a[0])==0)
		singleTalk(selectText);
	else if(strcmp(string,a[1])==0)
		groupTalk(selectText);
	else if(strcmp(string,a[2])==0)
		lookUserProfile(selectText);
        else if(strcmp(string,a[3])==0)
		moveToBlackList(selectText);
	else if(strcmp(string,a[4])==0)
		recoverFromBlackList(selectText);
        else if(strcmp(string,a[5])==0)
		moveToSpecialList(selectText);
        else if(strcmp(string,a[6])==0)
		recoverFromSpecialList(selectText);
}
//****************************************************/
/*
函数名:		lookUserProfile
参数：		str:资料字符串
返回值:	   void
功能：	       查看资料回调函数
*/
/**************************************************/
void lookUserProfile(gchar *str)
{
    struct sockaddr_in addr ;
    LinpopUser *temp = g_malloc(sizeof(*temp));
    gboolean result;
    char ip[16];
    int i;
    bzero(ip,sizeof(ip));
    g_print("str:%s\n",str);
    for(;*str != '(';str++);
    g_print("str2:%s\n",str);
    for(++str,i = 0;*str != ')';str++,i++)
        ip[i] = *str;
    g_print("ip:%s\n",ip);
    inet_aton(ip,&addr.sin_addr);
    result = getUserByIP(addr,temp);
    g_print("%d",result);
    if(result == FALSE){
        //g_print("No user at %s,cannot create chat window",ip);
        g_print("No this User!\n");
        g_free(temp);
        return ;
    }
    createProfileWindow(temp->user,temp->machine,ip,temp->nick);
    freeUser(temp);
	/*extern struct userinfo *head;
	struct userinfo *p = head;
  	const char *str = gtk_entry_get_text(entry);
	char node[50];
	char l[50];
	while(p!=NULL){
		strcpy(node,p->name);
		strcat(node,"(");
		strcat(node,p->ip);
		strcat(node,")");
		if(0 == strcmp(str,node)){
			printf("---------查看资料----------\n");
			printf("name:%s\n",p->name);
			printf("group:%s\n",p->group);
			printf("IP:%s\n",p->ip);
			printf("signature:%s\n",p->signature);
			printf("picture:%d\n",p->picture);
			get_usr_info(p->name,p->group,p->ip,p->signature);
		}
		p = p->next;
		strcpy(node,l);
	}*/

}
//****************************************************/
/*
函数名:	    singleTalk
参数：		str : 传入的单人聊天的信息
返回值:	   void
功能：	       单人聊天
*/
/**************************************************/
void singleTalk(gchar *str)
{
    struct sockaddr_in addr ;
    LinpopUser *temp = g_malloc(sizeof(*temp));
    gboolean result;
    char ip[16];
    int i;
    bzero(ip,sizeof(ip));
    g_print("str:%s\n",str);
    for(;*str != '(';str++);
    g_print("str2:%s\n",str);
    for(++str,i = 0;*str != ')';str++,i++)
        ip[i] = *str;
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
	//extern struct userinfo *head;
	//struct userinfo *p = head;
}
	/*char node[50];
	char l[50];
	while(p!=NULL){
		strcpy(node,p->name);
		strcat(node,"(");
		strcat(node,p->ip);
		strcat(node,")");
		if(0 == strcmp(str,node)){
			printf("---------单人聊天----------\n");
			printf("name:%s\n",p->name);
			printf("group:%s\n",p->group);
			printf("IP:%s\n",p->ip);
			printf("signature:%s\n",p->signature);
			printf("picture:%d\n",p->picture);
//add chat window call 2010-07-12
			if(p->textViewAll == NULL)
				createChatWindow(p->ip,p->name,NULL);
		}
		p = p->next;
		strcpy(node,l);
	}*/
//****************************************************/
/*
函数名:	    grouptalk
参数：		str : 组信息
返回值:	   void
功能：	    群组聊天
*/
/**************************************************/
void groupTalk(gchar *str)
{
    struct sockaddr_in addr ;
    LinpopUser *temp = g_malloc(sizeof(*temp));
    gboolean result;
    gchar *ip;
    GList *list = getGroupUserList(str);
    //assert(list != NULL);
    if(list == NULL){
        g_print("not this group!");
        return ;
    }
    for(; list != NULL;list = list->next){
            ip = (gchar *)list->data;
            //bzero(ip,sizeof(ip));
            //g_print("str:%s\n",str);
            //for(;*str != '(';str++);
            //g_print("str2:%s\n",str);
            //for(++str,i = 0;*str != ')';str++,i++)
                //ip[i] = *str;
            g_print("ip:%s\n",ip);
            inet_aton(ip,&addr.sin_addr);
            result = getUserByIP(addr,temp);
            //g_print("%d",result);
            if(result == FALSE){
                g_print("No user at %s,cannot create chat window",ip);
                g_free(temp);
                return ;
            }
            createChatWindow(inet_ntoa(temp->from.sin_addr),temp->user,SHOW);
    }
       freeUser(temp);
	/*while(p!=NULL){
		printf("************群组聊天***********\n\n");
		if(0 == strcmp(str,p->group)){
			printf("-----------------------\n");
			printf("name:%s\n",p->name);
			printf("group:%s\n",p->group);
			printf("IP:%s\n",p->ip);
			printf("signature:%s\n",p->signature);
			printf("picture:%d\n",p->picture);
			printf("-----------------------\n\n");
		}
		p = p->next;
	}*/
}
/**************************************************************/
//查看资料界面
/**************************************************************/
static void createProfileWindow(gchar *user,gchar *group,gchar *ip,gchar *absence)
{
	GtkWidget *usr_info;
	GtkWidget *table;
	GtkWidget *label;


	GtkWidget *pic;
	GtkWidget *usr;

	usr_info = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_size_request (GTK_WIDGET (usr_info), 300, 300);
	gtk_window_set_keep_above(GTK_WINDOW (usr_info),TRUE);
	gtk_window_set_skip_taskbar_hint(GTK_WINDOW (usr_info),TRUE);
	gtk_window_set_resizable(GTK_WINDOW (usr_info),FALSE);
	gtk_window_set_title (GTK_WINDOW (usr_info), "查看资料");

	pic = gtk_image_new_from_file ("Icon/1.gif");

	table = gtk_table_new(8,7,TRUE);
	gtk_container_add (GTK_CONTAINER (usr_info), table);
	label = gtk_label_new("用户名:");
	gtk_table_attach_defaults(GTK_TABLE (table), label, 0,2,0,1);

	label = gtk_label_new("群组:");
	gtk_table_attach_defaults(GTK_TABLE (table), label, 0,2,1,2);

	label = gtk_label_new("IP:");
	gtk_table_attach_defaults(GTK_TABLE (table), label, 0,2,2,3);

	label = gtk_label_new("个性签名:");
	gtk_table_attach_defaults(GTK_TABLE (table), label, 0,2,3,4);

	gtk_table_attach_defaults(GTK_TABLE (table), pic, 5,7,0,2);

	label = gtk_label_new(user);
	gtk_table_attach_defaults(GTK_TABLE (table), label, 2,5,0,1);

	label = gtk_label_new(group);
	gtk_table_attach_defaults(GTK_TABLE (table), label, 2,5,1,2);

	label = gtk_label_new(ip);
	gtk_table_attach_defaults(GTK_TABLE (table), label, 2,5,2,3);

	label = gtk_label_new(absence);
	gtk_table_attach_defaults(GTK_TABLE (table), label, 2,5,3,4);

	gtk_widget_show_all(usr_info);
}
/*******************************************************/
// 对用户组进行修改，需要对配置文件的读写
/*******************************************************/
void moveToBlackList(gchar *str)
{
    struct sockaddr_in addr ;
    LinpopUser *temp = g_malloc(sizeof(*temp));
    gboolean result;
    char ip[16];
    int i;
    bzero(ip,sizeof(ip));
    g_print("str:%s\n",str);
    for(;*str != '(';str++);
    g_print("str2:%s\n",str);
    for(++str,i = 0;*str != ')';str++,i++)
        ip[i] = *str;
    g_print("ip:%s\n",ip);
    inet_aton(ip,&addr.sin_addr);
    result = getUserByIP(addr,temp);
    g_print("%d",result);
    if(result == FALSE){
        g_print("No user at %s,cannot create chat window",ip);
        g_free(temp);
        return ;
    }
    //createChatWindow(inet_ntoa(temp->from.sin_addr),temp->user,SHOW);
    setGroup(addr,"黑名单");
    listRefresh();
    freeUser(temp);
	//extern struct userinfo *head;
	//struct userinfo *p = head;
}
/*******************************************************/
// 将用户移至黑名单，需要对配置文件进行读写，将 改接受信息屏蔽
/*******************************************************/
void recoverFromBlackList(gchar *str)
{
    struct sockaddr_in addr ;
    LinpopUser *temp = g_malloc(sizeof(*temp));
    gboolean result;
    char ip[16];
    int i;
    bzero(ip,sizeof(ip));
    g_print("str:%s\n",str);
    for(;*str != '(';str++);
    g_print("str2:%s\n",str);
    for(++str,i = 0;*str != ')';str++,i++)
        ip[i] = *str;
    g_print("ip:%s\n",ip);
    inet_aton(ip,&addr.sin_addr);
    result = getUserByIP(addr,temp);
    g_print("%d",result);
    if(result == FALSE){
        g_print("No user at %s,cannot create chat window",ip);
        g_free(temp);
        return ;
    }
    setGroup(addr,temp->machine);
    listRefresh();
    freeUser(temp);
	//extern struct userinfo *head;
	//struct userinfo *p = head;
}
void moveToSpecialList(gchar *str)
{
    struct sockaddr_in addr ;
    LinpopUser *temp = g_malloc(sizeof(*temp));
    gboolean result;
    char ip[16];
    int i;
    bzero(ip,sizeof(ip));
    g_print("str:%s\n",str);
    for(;*str != '(';str++);
    g_print("str2:%s\n",str);
    for(++str,i = 0;*str != ')';str++,i++)
        ip[i] = *str;
    g_print("ip:%s\n",ip);
    inet_aton(ip,&addr.sin_addr);
    result = getUserByIP(addr,temp);
    g_print("%d",result);
    if(result == FALSE){
        g_print("No user at %s,cannot create chat window",ip);
        g_free(temp);
        return ;
    }
    setGroup(addr,"特别好友");
    listRefresh();
    freeUser(temp);
	//extern struct userinfo *head;
	//struct userinfo *p = head;
}
void recoverFromSpecialList(gchar *str)
{
    struct sockaddr_in addr ;
    LinpopUser *temp = g_malloc(sizeof(*temp));
    gboolean result;
    char ip[16];
    int i;
    bzero(ip,sizeof(ip));
    g_print("str:%s\n",str);
    for(;*str != '(';str++);
    g_print("str2:%s\n",str);
    for(++str,i = 0;*str != ')';str++,i++)
        ip[i] = *str;
    g_print("ip:%s\n",ip);
    inet_aton(ip,&addr.sin_addr);
    result = getUserByIP(addr,temp);
    g_print("%d",result);
    if(result == FALSE){
        g_print("No user at %s,cannot create chat window",ip);
        g_free(temp);
        return ;
    }
    setGroup(addr,temp->machine);
    listRefresh();
    freeUser(temp);
	//extern struct userinfo *head;
	//struct userinfo *p = head;
}
