/********************************************/
/*名称：i_main_window.c
/*描述： 1、该文件定义了程序中用于创建主界面的函数
	2、定义了程序中用到的部分全局变量
/*作者：
/*日期：
/********************************************/
#ifndef __I_MAIN_WINDOW_
#define __I_MAIN_WINDOW_
#include <assert.h>
#include "m_user.h"
#include "i_main_window.h"
#include "i_utils.h"
#include "i_main_callbacks.h"
#include "i_main_stmenu.h"
#include "i_main_window.h"
#include "m_linpop.h"
#include "g_properties.h"
#include "i_main_cmenu.h"
#include "i_setting.h"
#define TEXT_SIZE 1024

GtkWidget *window;
GtkWidget *vbox_mid;
GtkWidget *vbox_main;
GtkStatusIcon *trayIcon;
GtkWidget *entry;//中间过度控件，用于获取选项的值
GtkWidget *view;//分组显示好友列表的控件
GtkWidget *online;		//显示在线状态的文字信息
GtkWidget *online_state;	//显示在线状态的图标信息
GtkWidget *photo;		//用户头像
GtkWidget *name;		//用户名
GtkWidget *usr_state;		//心情
gchar *selectText;

static int printUser(LinpopUser *user,void *parm);
static GtkWidget *get_vbox_mid();
static GtkTreeModel *createTreeModel();
static GtkWidget * createViewAndModel (void);
static GtkWidget* get_hbox_top();
static GtkWidget* get_hbox_bottom();
/****************************************************/
/*
函数名:		creatMainWindow
参数：		void
返回值:		void
功能：		 创建主界面
*/
/**************************************************/
void createMainWindow(void)
{
	//变量声明
	GtkWidget *hbox_top,*hbox_bottom;
	GtkWidget *name,*photo;
	GtkWidget *menu, *menuItemView, *menuItemExit,*menuItemHelp;
	gchar buf[TEXT_SIZE];

	//初始化部分
        selectText =  g_malloc(TEXT_SIZE);
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_size_request (GTK_WIDGET (window), 220, 600);
	gtk_window_move(GTK_WINDOW (window),1200,0);
	gtk_window_set_keep_above(GTK_WINDOW (window),TRUE);
	gtk_window_set_skip_taskbar_hint(GTK_WINDOW (window),TRUE);
	gtk_window_set_resizable(GTK_WINDOW (window),FALSE);
	gtk_window_set_title (GTK_WINDOW (window), "NEU_MSN");
	g_signal_connect (G_OBJECT (window), "destroy",G_CALLBACK (gtk_main_quit), NULL);
	//g_signal_connect_swapped (G_OBJECT (window), "delete_event",G_CALLBACK (gtk_widget_destroy), G_OBJECT (window));

	GdkPixbuf *pixbuf = NULL;
	GdkBitmap *bitmap = NULL;
	GdkPixmap *pixmap = NULL;
	gtk_widget_set_app_paintable(GTK_WIDGET (window), TRUE);
	gtk_widget_realize(GTK_WIDGET (window));
	pixbuf = createPixbufByName("Icon/bg3.jpg");      // gdk函数读取png文件
	gdk_pixbuf_render_pixmap_and_mask(pixbuf, &pixmap, &bitmap, 128);   // alpha小于128认为透明
	gtk_widget_shape_combine_mask(GTK_WIDGET (window), bitmap, 0, 0);   // 设置透明蒙板
	gdk_window_set_back_pixmap(window->window, pixmap, FALSE);          // 设置窗口背景
	g_object_unref(pixbuf);
	g_object_unref(bitmap);
	g_object_unref(pixmap);

	gtk_window_set_icon(GTK_WINDOW (window),createPixbufByName("pic/logo.bmp"));
	//ti = (tray_icon*)malloc(sizeof(tray_icon));
	//任务栏图标
	trayIcon  = gtk_status_icon_new_from_file ("Icon/1.png");
	menu = gtk_menu_new();
	menuItemExit = gtk_menu_item_new_with_label ("退出");
	menuItemHelp = gtk_menu_item_new_with_label ("帮助");
	menuItemView = gtk_menu_item_new_with_label ("主面板");
	g_signal_connect (G_OBJECT (menuItemView), "activate", G_CALLBACK (trayView), window);
	g_signal_connect (G_OBJECT (menuItemExit), "activate", G_CALLBACK (gtk_main_quit), NULL);
	g_signal_connect (G_OBJECT (menuItemHelp), "activate", G_CALLBACK (show_about), NULL);
	gtk_menu_shell_append (GTK_MENU_SHELL (menu), menuItemView);
	gtk_menu_shell_append (GTK_MENU_SHELL (menu), menuItemHelp);
	gtk_menu_shell_append (GTK_MENU_SHELL (menu), menuItemExit);
	gtk_widget_show_all (menu);
	gtk_status_icon_set_tooltip (trayIcon, "NEU_MSN");
	g_signal_connect(GTK_STATUS_ICON (trayIcon), "activate", GTK_SIGNAL_FUNC (trayIconActivated), window);
	g_signal_connect(GTK_STATUS_ICON (trayIcon), "popup-menu", GTK_SIGNAL_FUNC (trayIconPopup), menu);
	gtk_status_icon_set_visible(trayIcon, TRUE);
	gtk_status_icon_set_blinking (trayIcon,	FALSE);
	g_signal_connect (G_OBJECT (window), "window-state-event", G_CALLBACK (window_state_event),NULL);
	//界面部分
	vbox_main = gtk_vbox_new (FALSE,0);
	gtk_container_add (GTK_CONTAINER (window), vbox_main);
	hbox_top  = get_hbox_top (FALSE,0);
	gtk_box_pack_start(GTK_BOX (vbox_main),hbox_top,FALSE,TRUE,0);
	//IP输入
	entry = gtk_entry_new ();
	gtk_widget_set_tooltip_text(entry,"搜索好友");
	gtk_entry_set_max_length (GTK_ENTRY (entry), 20);
	gtk_widget_set_size_request (GTK_WIDGET (entry),195,25);
	g_signal_connect (G_OBJECT (entry), "icon-press",G_CALLBACK (searchFriend),entry);
	g_signal_connect (G_OBJECT (entry), "backspace",G_CALLBACK (clearEntry),entry);
	gtk_entry_set_icon_from_pixbuf(GTK_ENTRY (entry),GTK_ENTRY_ICON_SECONDARY,createPixbufByName("Icon/find.svg"));
	gtk_entry_set_icon_tooltip_text(GTK_ENTRY (entry),GTK_ENTRY_ICON_SECONDARY,"搜索好友");
	numToStr(buf,getUserNumber());
	strcat(buf," friends online");
	gtk_entry_set_text (GTK_ENTRY (entry), buf);
	gtk_box_pack_start (GTK_BOX (vbox_main), entry, FALSE, FALSE, 0);
	//用户、群组、最近联系人
	//vbox_mid = get_vbox_mid();
	get_vbox_mid();
	gtk_box_pack_start(GTK_BOX (vbox_main),vbox_mid, FALSE, TRUE, 0);
	//底层按钮
	hbox_bottom = get_hbox_bottom();
	gtk_box_pack_end(GTK_BOX (vbox_main),hbox_bottom, FALSE, TRUE, 0);

	gtk_widget_show_all (window);
	//gtk_main();
}
/****************************************************/
/*
函数名:		remove_all
参数：		void
返回值:		void
功能：	    移除所有树形节点
*/
/**************************************************/
void remove_all(void)
{

	GtkTreeStore *store;
	GtkTreeModel *model;
	GtkTreeIter  iter;

	store = GTK_TREE_STORE(gtk_tree_view_get_model(
				GTK_TREE_VIEW (view)));
	model = gtk_tree_view_get_model (GTK_TREE_VIEW (view));

	if (gtk_tree_model_get_iter_first(model, &iter) == FALSE)
	return;
	gtk_tree_store_clear(store);
	printf("列表已清空!\n\n");
}
/****************************************************/
/*
函数名:		get_vbox_mid
参数：		void
返回值:		GtkWidget ＊
功能：	    建立 主界面中间部分
*/
/**************************************************/
static GtkWidget *get_vbox_mid()
{
	GtkWidget *hbox_mid;
  	GtkWidget *talk;
  	GtkWidget *look;
  	GtkWidget *group_talk;
  	GtkWidget *sw;
  	GtkTreeSelection *selection;
	GtkWidget *menu_right;
	  vbox_mid = gtk_vbox_new(FALSE, 0);
	  gtk_widget_set_size_request (GTK_WIDGET (vbox_mid), 220,470);
	  view = createViewAndModel();
	  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(view));
	  gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (view), FALSE);
	  sw = gtk_scrolled_window_new(NULL, NULL);
	  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(sw),GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	  gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW(sw),GTK_SHADOW_ETCHED_IN);
	  gtk_box_pack_start(GTK_BOX(vbox_mid), sw, TRUE, TRUE, 0);
	  gtk_container_add(GTK_CONTAINER (sw), view);
	  g_signal_connect(selection, "changed", G_CALLBACK(onChanged), selectText);
	  g_signal_connect(view,"row-activated",startChat,NULL);
//右键菜单
	menu_right = getRightMenu();
	addMenuToPopup(view,GTK_MENU(menu_right));
	return NULL;
}
/****************************************************/
/*
函数名:		createTreeModel
参数：		void
返回值:		GtkTreeModel *
功能：	    创建树形模型，将数据填入
*/
/**************************************************/
static GtkTreeModel *createTreeModel()
{
    GtkTreeStore *treestore;
    GtkTreeIter toplevel, child;
    treestore = gtk_tree_store_new(NUM_COLS,G_TYPE_STRING);
    gchar *p = NULL;
    gchar *q = NULL;
    int i,j;
    struct sockaddr_in addr;
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
    return GTK_TREE_MODEL(treestore);
}
/****************************************************/
/*
函数名:		createViewAndModel
参数：		void
返回值:		GtkTreeModel *
功能：	    创建树形模型，构建视图
*/
/**************************************************/
static GtkWidget * createViewAndModel (void)
{
    GtkTreeViewColumn *col;
    GtkCellRenderer *renderer;
    GtkWidget *view;
    GtkTreeModel *model;
    GtkTreeStore *treestore;
    GtkTreeIter toplevel, child;
    gchar *p = NULL;
    gchar *q = NULL;
    int i,j;
    struct sockaddr_in addr;
    GList *group = NULL;
    GList *user = NULL;
    LinpopUser *temp ;
    char buf[256];
    gboolean result;
    treestore = gtk_tree_store_new(NUM_COLS,G_TYPE_STRING);
    //userListForeach(printUser,NULL);
    for(group = getGroupList();group != NULL;group = group->next){
        p = (gchar *)group->data;
	gtk_tree_store_append(treestore, &toplevel, NULL);
	gtk_tree_store_set(treestore, &toplevel,COLUMN, p,-1);
        g_print("group:%s\n",p);
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
            g_print("q is:%s\n",q);
		    gtk_tree_store_append(treestore, &child, &toplevel);
		    gtk_tree_store_set(treestore, &child,COLUMN,q,-1);
            g_print("user:%s\n",q);
            freeUser(temp);

	}
    }

    view = gtk_tree_view_new();
    col = gtk_tree_view_column_new();
    gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);

    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_column_pack_start(col, renderer, TRUE);
    gtk_tree_view_column_add_attribute(col,renderer, "text", COLUMN);

    model = GTK_TREE_MODEL(treestore);
    gtk_tree_view_set_model(GTK_TREE_VIEW(view), model);
    g_object_unref(model);

    return view;
}
/****************************************************/
/*
函数名:		get_hbox_top
参数：		void
返回值:		GtkWidget *
功能：	    创建主界面上部部分
*/
/**************************************************/
static GtkWidget* get_hbox_top()
{

	GtkWidget *hbox_top;
	GtkWidget *vbox_top;
	GtkWidget *hbox_usr;
	GtkWidget *menu_state;

	//char *user_signature = LinpopGetAbsence();
	char *user_signature = "Please input status";
	int user_pic = 1;
	char *user_name = getUserName();

	hbox_top  = gtk_hbox_new (FALSE,0);
	vbox_top  = gtk_vbox_new (FALSE,0);
	hbox_usr  = gtk_hbox_new (FALSE,0);
//用户头像
	photo = createImageButton("Icon/1.gif");
	gtk_button_set_relief(GTK_BUTTON(photo),GTK_RELIEF_NONE);
	gtk_button_set_focus_on_click(GTK_BUTTON(photo), FALSE);
	gtk_widget_set_size_request (GTK_WIDGET (photo), 50,53);
	g_signal_connect(G_OBJECT(photo), "clicked",G_CALLBACK (setting),NULL);
	gtk_box_pack_start (GTK_BOX (hbox_top),photo,FALSE,TRUE,0);
//用户名
	name = gtk_label_new(NULL);
	gtk_label_set_text(GTK_LABEL(name),user_name);
	gtk_box_pack_start (GTK_BOX (hbox_usr),name,FALSE,TRUE,0);
//在线状态
	online = gtk_label_new("我在线上");
	online_state = createImageButton("Icon/available.svg");
	gtk_button_set_relief(GTK_BUTTON(online_state),GTK_RELIEF_NONE);
	menu_state = getStateMenu();
    	gtk_box_pack_start(GTK_BOX (hbox_usr),online_state,FALSE,TRUE,0);
	gtk_box_pack_end(GTK_BOX (hbox_usr),online,FALSE,TRUE,0);
	g_signal_connect_swapped (G_OBJECT (online_state), "event",G_CALLBACK (doStateButtonPress),G_OBJECT(menu_state));
	gtk_box_pack_start (GTK_BOX (vbox_top),hbox_usr,FALSE,TRUE,0);
//用户状态
	usr_state = gtk_button_new_with_label(user_signature);
	gtk_button_set_relief(GTK_BUTTON(usr_state ),GTK_RELIEF_NONE);
	gtk_widget_set_tooltip_text(usr_state ,"与好友分享你的心情:");
	gtk_widget_set_size_request (GTK_WIDGET (usr_state),120,26);
	g_signal_connect(G_OBJECT(usr_state), "clicked",G_CALLBACK (setting),NULL);
	gtk_box_pack_start (GTK_BOX (vbox_top), usr_state, FALSE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (hbox_top), vbox_top, TRUE, TRUE, 0);
	return hbox_top;
}
/****************************************************/
/*
函数名:		get_hbox_bottom
参数：		void
返回值:		GtkWidget *
功能：	    创建主界面底部部分
*/
/**************************************************/
static GtkWidget* get_hbox_bottom()
{
	GtkWidget *hbox_bottom;
	GtkWidget *swap_style;
	GtkWidget *menu_style;
	GtkWidget *help;
	GtkWidget *time;
	GtkWidget *refresh;
	GtkWidget *set_soft;
	char buf[128];
	hbox_bottom = gtk_hbox_new (FALSE,0);
	gtk_widget_set_size_request (GTK_WIDGET (hbox_bottom),200,30);
//时间日期
	getCurrentDay(buf,sizeof(buf));
	getCurrentTime(buf,sizeof(buf));
	time = gtk_image_new_from_file ("Icon/clock.svg");
	gtk_widget_set_tooltip_text(time,buf);
	gtk_box_pack_start(GTK_BOX (hbox_bottom),time,FALSE,TRUE,0);
//更换皮肤
	swap_style = createImageButton("Icon/style.svg");
	gtk_button_set_relief(GTK_BUTTON(swap_style),GTK_RELIEF_NONE);
	menu_style = getStyleMenu();
        gtk_widget_set_tooltip_text(swap_style,"更换皮肤");
    	gtk_box_pack_end(GTK_BOX (hbox_bottom),swap_style,FALSE,TRUE,0);
	g_signal_connect_swapped (G_OBJECT (swap_style), "event",G_CALLBACK (doStylePress),G_OBJECT (menu_style));
//软件帮助
	help = createImageButton("Icon/contents.svg");
 	gtk_button_set_relief(GTK_BUTTON(help),GTK_RELIEF_NONE);
	gtk_button_set_focus_on_click(GTK_BUTTON(help), FALSE);
	g_signal_connect(G_OBJECT(help), "clicked",G_CALLBACK (show_about),NULL);
	gtk_widget_set_tooltip_text(help,"软件帮助");
    	gtk_box_pack_end(GTK_BOX (hbox_bottom),help,FALSE,TRUE,0);
//刷新
	refresh = createImageButton("Icon/refresh.svg");
 	gtk_button_set_relief(GTK_BUTTON(refresh),GTK_RELIEF_NONE);
	gtk_button_set_focus_on_click(GTK_BUTTON(refresh), FALSE);
	g_signal_connect(G_OBJECT(refresh), "clicked",G_CALLBACK (listRefresh),NULL);
	gtk_widget_set_tooltip_text(refresh,"刷新");
    	gtk_box_pack_end(GTK_BOX (hbox_bottom),refresh,FALSE,TRUE,0);
//软件设置
	set_soft = createImageButton("Icon/editor.svg");
	gtk_button_set_focus_on_click(GTK_BUTTON(set_soft), FALSE);
 	gtk_button_set_relief(GTK_BUTTON(set_soft),GTK_RELIEF_NONE);
	g_signal_connect(G_OBJECT(set_soft), "clicked",G_CALLBACK (setting),NULL);
	gtk_widget_set_tooltip_text(set_soft,"软件设置");
    	gtk_box_pack_end(GTK_BOX (hbox_bottom),set_soft,FALSE,TRUE,0);
	return hbox_bottom;
}
/****************************************************/
/*
函数名:		printUser
参数：		user ： 传入的用户 信息
返回值:		int
功能：	    打印用户信息
*/
/**************************************************/
static int printUser(LinpopUser *user,void *parm)
{
    g_print("user:%s\n",user->user);
    g_print("machine:%s\n",user->machine);
    g_print("nick:%s\n",user->nick);
    g_print("group:%s\n",user->group);
    return 0;
}
#endif
