/********************************************/
/*名称：i_setting.c
/*描述 : 软件设置界面与回调函数的建立
/*作者：
/*日期：
/********************************************/
#include <gtk/gtk.h>
#include <string.h>
#include "i_setting.h"
#include "i_main_window.h"
#include "i_utils.h"
GtkWidget *fcbutton;
int charge;
/******************************************************************
创建list的模型
*******************************************************************/
ListItem t[8] = {{"Icon/pix1.png","个人资料设置"},{"Icon/pix2.png","系统设置"},{"Icon/pix3.png","发送接受设置"},{"Icon/pix4.png","群设置"},{"Icon/pix5.png","功能设置"},{"Icon/pix6.png","网络设置 "},{"Icon/pix7.png","个性设置"}};
Item_Image g[10] = {{"头像一","Icon/1.gif"},{"头像二","Icon/2.gif"},{"头像三","Icon/3.gif"},{"头像四","Icon/4.gif"},{"头像五","Icon/5.gif"},{"头像六","Icon/6.gif"},{"头像七","Icon/7.gif"},{"头像八","Icon/8.gif"},{"头像九","Icon/9.gif"},{"头像十","Icon/10.gif"}};
GtkListStore* create_list_model(void)
{
	GtkListStore *list_store;
	GtkTreeIter iter;
	gint i;
	GdkPixbuf *pixbuf;
	list_store  = gtk_list_store_new(N_COLUMN,GDK_TYPE_PIXBUF,G_TYPE_STRING);
	for(i=0;i<7;i++)
	{
		pixbuf = gdk_pixbuf_new_from_file(t[i].pix, NULL);
		gtk_list_store_append(list_store,&iter);
		gtk_list_store_set(list_store,&iter,PIXBUF_COLUMN,pixbuf,TEXT_COLUMN,t[i].text,-1);
	}
	return list_store;
}

/******************************************************
创建list
******************************************************/
GtkWidget* create_list(GtkListStore *list_store)
{
	GtkWidget *view;
	GtkTreeModel *model;
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;

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
	return view;
}

GtkWidget *create_item(gint i)
{
	GtkWidget* item;
	GtkWidget* image;
	GtkWidget* hbox;

	hbox = gtk_hbox_new(FALSE,0);
	switch(i)
	{
	case 1:
		image = gtk_image_new_from_file(g[0].fcimage);
		break;
	case 2:
		image = gtk_image_new_from_file(g[1].fcimage);
		break;
	case 3:
		image = gtk_image_new_from_file(g[2].fcimage);
		break;
	case 4:
		image = gtk_image_new_from_file(g[3].fcimage);
		break;
	case 5:
		image = gtk_image_new_from_file(g[4].fcimage);
		break;
	case 7:
		image = gtk_image_new_from_file(g[6].fcimage);
		break;
	case 8:
		image = gtk_image_new_from_file(g[7].fcimage);
		break;
	case 9:
		image = gtk_image_new_from_file(g[8].fcimage);
		break;
	case 10:
		image = gtk_image_new_from_file(g[9].fcimage);
		break;
	case 6:
		image = gtk_image_new_from_file(g[5].fcimage);
		break;
	}
	gtk_box_pack_start(GTK_BOX(hbox),image,FALSE,FALSE,2);
	item = gtk_list_item_new();
	gtk_container_add(GTK_CONTAINER(item),hbox);
	gtk_widget_show_all(item);
	return item;
}



/***************************************************************
回调函数，点击list对应显示notebook
***************************************************************/
void  on_change(GtkWidget *widget, gpointer notebook)
{
  GtkTreeIter iter;
  GtkTreeModel *model;
  char *value;
	int i;


  if (gtk_tree_selection_get_selected(
      GTK_TREE_SELECTION(widget), &model, &iter)) {

    gtk_tree_model_get(model, &iter,TEXT_COLUMN, &value,  -1);
    //gtk_label_set_text(GTK_LABEL(label), value);
	for(i=0;i<7;i++){
		if(0 == strcmp(t[i].text,value))
			gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook),i);
	}
    g_free(value);
  }

}
/*******************************************************/
//输入用户名文本框，改变 用户 名
/*******************************************************/
void entry_name(GtkWidget *widget,gpointer entry)
{
	extern GtkWidget *name;		//用户名
	const gchar *text;
	text = gtk_entry_get_text(GTK_ENTRY(entry));
	gtk_entry_set_text(GTK_ENTRY(entry),text);
	gtk_label_set_text(GTK_LABEL(name),text);
}
/*******************************************************/
// 输入用户组文本框，改变用户组
/*******************************************************/
void entry_group(GtkWidget *widget,gpointer entry)
{
	const gchar *text;
	text = gtk_entry_get_text(GTK_ENTRY(entry));

	gtk_entry_set_text(GTK_ENTRY(entry),text);
}
/*******************************************************/
// 输入用户心情信息，改变用户心情信息
/*******************************************************/
void entry_sig(GtkWidget *widget,gpointer entry)
{

	extern GtkWidget *usr_state;		//心情
	const gchar *text;
	text = gtk_entry_get_text(GTK_ENTRY(entry));
	gtk_entry_set_text(GTK_ENTRY(entry),text);
	gtk_button_set_label(GTK_BUTTON(usr_state),text);
}
/*******************************************************/
//设置设置软件界面取消按钮回调 函数
/*******************************************************/
void set_soft_quit(GtkWidget *widget,GtkWidget *set_soft)
{
	charge = 0;
	gtk_widget_destroy(set_soft);
}
/*******************************************************/
//生成 设置软件界面应用按钮回调函数
/*******************************************************/
void set_soft_apply(GtkWidget *widget,gpointer entry)
{
   /* const gchar * string;
    string = gtk_entry_get_text(GTK_ENTRY(entry));
    printf("%s",string);
    strcpy(user_signature,string);
    gtk_entry_set_text(GTK_ENTRY(entry),text);
	gtk_button_set_label(GTK_BUTTON(usr_state),text);*/
	//entry_name(set_soft,);
}
/*******************************************************/
// 头像下拉菜单选项的回调函数，改变用户 头像
/*******************************************************/
void my_callback_function(GtkWidget *widget,gpointer combo)
{
	const gchar *string;
	GtkWidget *image;
	GtkWidget *image1;

	int i;
	string = gtk_entry_get_text (GTK_ENTRY (GTK_COMBO (combo)->entry));
	for(i=0;i<10;i++)
	{
		if(0 == strcmp(g[i].fcname,string))
		{
			image = gtk_image_new_from_file(g[i].fcimage);
			image1 = gtk_image_new_from_file(g[i].fcimage);
			gtk_button_set_image(GTK_BUTTON(photo),image);
			gtk_button_set_image(GTK_BUTTON(fcbutton),image1);
			printf("%s\n",g[i].fcimage);
		}
	}
}
/*******************************************************/
// 生成设置软件界面确定按钮回调函数
/*******************************************************/
void set_soft_ok(GtkWidget *widget,GtkWidget *set_soft)
{

    charge = 0;
	gtk_widget_destroy(set_soft);
}
/******************************************************
 //构建设置软件界面
******************************************************/
void setting()
{
	GtkWidget *soft_set;
	GtkWidget *hbox;
	GtkWidget *hbox1;
	GtkWidget *vbox;
	GtkWidget *notebook;
	GtkWidget *table;
	GtkWidget *table1;
	GtkWidget *view;
	GtkListStore *model;
	GtkWidget *frame;
	GtkWidget *label;
	GtkWidget *label1;
	GtkWidget *button;
	GtkWidget *cancel_button;
	GtkWidget *apply_button;
	GtkWidget *ok_button;
	GtkWidget *entry;
	GtkWidget *user_entry;
	GtkWidget *group_entry;
	GtkWidget *sig_entry;
	GtkWidget *checkbutton;
	GtkWidget* item;
	GtkWidget* image;
	GtkWidget *combo;
	GtkWidget *radio;
	GtkWidget *vbox1;

	GSList *group;
	gint i;
	GtkTreeSelection *selection;
	//static GdkColor blue = {0,0,0,0xffff};

	if(charge == 0){
//gtk_init(&argc,&argv);
	//创建主窗口
	charge = 1;
	soft_set = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	//g_signal_connect(G_OBJECT(soft_set),"delete_event",G_CALLBACK(gtk_main_quit),NULL);
	gtk_window_set_title(GTK_WINDOW(soft_set),"软件设置");
	gtk_window_set_position(GTK_WINDOW(soft_set),GTK_WIN_POS_CENTER);
	gtk_container_set_border_width(GTK_CONTAINER(soft_set),5);
	gtk_widget_set_size_request (soft_set, 500, 420);
gtk_window_set_icon(GTK_WINDOW (soft_set),createPixbufByName("pic/logo.bmp"));
	vbox = gtk_vbox_new(FALSE, 0);
	hbox1 = gtk_hbox_new(FALSE, 0);
	hbox = gtk_hbox_new(FALSE, 0);

	gtk_container_add(GTK_CONTAINER(soft_set), vbox);

	model = create_list_model();
	view = create_list(model);

	gtk_box_pack_start(GTK_BOX(hbox), view, FALSE, FALSE, 2);
	//创建notebook
	notebook = gtk_notebook_new ();
	gtk_notebook_set_tab_pos (GTK_NOTEBOOK (notebook),GTK_POS_LEFT);
	gtk_notebook_set_show_tabs(GTK_NOTEBOOK (notebook),FALSE);
	gtk_notebook_set_show_border(GTK_NOTEBOOK (notebook),FALSE);
	gtk_box_pack_start(GTK_BOX(hbox), notebook, TRUE, TRUE, 2);
	for (i = 0; i < 7; i++) {
		switch(i)
		{
		//个人资料设置
		case 0:
       			{
			frame = gtk_frame_new (t[i].text);

			table = gtk_table_new(7,2,TRUE);
			gtk_container_set_border_width(GTK_CONTAINER(table),1);
			gtk_container_add(GTK_CONTAINER(frame),table);

			label = gtk_label_new ("用户名：");
			gtk_table_attach_defaults(GTK_TABLE(table),label,0,1,0,1);
			user_entry = gtk_entry_new();
			gtk_table_attach_defaults(GTK_TABLE (table), user_entry, 1,4,0,1);
			g_signal_connect(GTK_OBJECT(user_entry),"activate",G_CALLBACK(entry_name),(gpointer)user_entry);

			label = gtk_label_new ("组名：");
			gtk_table_attach_defaults(GTK_TABLE(table),label,0,1,1,2);
			group_entry = gtk_entry_new();
			gtk_table_attach_defaults(GTK_TABLE (table), group_entry, 1,4,1,2);
			g_signal_connect(GTK_OBJECT(user_entry),"activate",G_CALLBACK(entry_group),(gpointer)user_entry);

			label = gtk_label_new ("个性签名：");
			gtk_table_attach_defaults(GTK_TABLE(table),label,0,1,2,3);
			sig_entry = gtk_entry_new();
			gtk_table_attach_defaults(GTK_TABLE (table), sig_entry, 1,4,2,3);
			g_signal_connect(GTK_OBJECT(user_entry),"activate",G_CALLBACK(entry_sig),(gpointer)user_entry);

			label = gtk_label_new ("头像：");
			gtk_table_attach_defaults(GTK_TABLE(table),label,0,1,3,4);

			combo = gtk_combo_new();
			//image = gtk_image_new_from_file("Icon/1.gif");
			item = create_item(1);
			gtk_combo_set_item_string(GTK_COMBO(combo),GTK_ITEM(item),g[0].fcname);
			gtk_container_add(GTK_CONTAINER(GTK_COMBO(combo)->list),item);

			item = create_item(2);
			gtk_combo_set_item_string(GTK_COMBO(combo),GTK_ITEM(item),g[1].fcname);
			gtk_container_add(GTK_CONTAINER(GTK_COMBO(combo)->list),item);

			item = create_item(3);
			gtk_combo_set_item_string(GTK_COMBO(combo),GTK_ITEM(item),g[2].fcname);
			gtk_container_add(GTK_CONTAINER(GTK_COMBO(combo)->list),item);

			item = create_item(4);
			gtk_combo_set_item_string(GTK_COMBO(combo),GTK_ITEM(item),g[3].fcname);
			gtk_container_add(GTK_CONTAINER(GTK_COMBO(combo)->list),item);

			item = create_item(5);
			gtk_combo_set_item_string(GTK_COMBO(combo),GTK_ITEM(item),g[4].fcname);
			gtk_container_add(GTK_CONTAINER(GTK_COMBO(combo)->list),item);

			item = create_item(6);
			gtk_combo_set_item_string(GTK_COMBO(combo),GTK_ITEM(item),g[5].fcname);
			gtk_container_add(GTK_CONTAINER(GTK_COMBO(combo)->list),item);

			item = create_item(7);
			gtk_combo_set_item_string(GTK_COMBO(combo),GTK_ITEM(item),g[6].fcname);
			gtk_container_add(GTK_CONTAINER(GTK_COMBO(combo)->list),item);

			item = create_item(8);
			gtk_combo_set_item_string(GTK_COMBO(combo),GTK_ITEM(item),g[7].fcname);
			gtk_container_add(GTK_CONTAINER(GTK_COMBO(combo)->list),item);

			item = create_item(9);
			gtk_combo_set_item_string(GTK_COMBO(combo),GTK_ITEM(item),g[8].fcname);
			gtk_container_add(GTK_CONTAINER(GTK_COMBO(combo)->list),item);

			item = create_item(10);
			gtk_combo_set_item_string(GTK_COMBO(combo),GTK_ITEM(item),g[9].fcname);
			gtk_container_add(GTK_CONTAINER(GTK_COMBO(combo)->list),item);
			gtk_table_attach_defaults(GTK_TABLE (table), combo, 2,3,3,4);

			fcbutton = createImageButton("Icon/1.gif");
			gtk_button_set_relief(GTK_BUTTON(fcbutton),GTK_RELIEF_NONE);
//设置头像改变 函数
			g_signal_connect (G_OBJECT (GTK_COMBO (combo)->entry), "changed",G_CALLBACK (my_callback_function), (gpointer) combo);
			g_signal_connect (G_OBJECT (GTK_ENTRY (user_entry)), "changed",G_CALLBACK (entry_name), (gpointer) user_entry);
			g_signal_connect (G_OBJECT (GTK_ENTRY (sig_entry)), "changed",G_CALLBACK (entry_sig), (gpointer) sig_entry);
       			gtk_table_attach_defaults(GTK_TABLE (table),fcbutton, 1,2,3,4);
			label = gtk_label_new (t[i].text);
       			gtk_notebook_append_page (GTK_NOTEBOOK (notebook), frame, label);
			}
			break;
		//系统设置
		case 1:{
			table = gtk_table_new(14,8,TRUE);
			checkbutton = gtk_check_button_new_with_label ("开机自动运行");
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton),TRUE);
			gtk_table_attach_defaults(GTK_TABLE (table), checkbutton, 0,8,0,1);

			checkbutton = gtk_check_button_new_with_label ("开机自动运行时，程序自动最小化");
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton),TRUE);
			gtk_table_attach_defaults(GTK_TABLE (table), checkbutton, 0,8,1,2);

			checkbutton = gtk_check_button_new_with_label ("双击运行程序时，程序自动最小化");
			gtk_table_attach_defaults(GTK_TABLE (table), checkbutton, 0,8,2,3);

			checkbutton = gtk_check_button_new_with_label ("左键单击托盘图标打开主界面");
			gtk_table_attach_defaults(GTK_TABLE (table), checkbutton, 0,8,3,4);

			checkbutton = gtk_check_button_new_with_label ("程序运行过程中，占用内存自动最小化");
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton),TRUE);
			gtk_table_attach_defaults(GTK_TABLE (table), checkbutton, 0,8,4,5);

			checkbutton = gtk_check_button_new_with_label ("自动检查最新版本");
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton),TRUE);
			gtk_table_attach_defaults(GTK_TABLE (table), checkbutton, 0,8,5,6);

			checkbutton = gtk_check_button_new_with_label ("主面板自动隐藏");
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton),TRUE);
			gtk_table_attach_defaults(GTK_TABLE (table), checkbutton, 0,8,6,7);

			checkbutton = gtk_check_button_new_with_label ("主面板总位于最上面(下次运行有效)");
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton),TRUE);
			gtk_table_attach_defaults(GTK_TABLE (table), checkbutton, 0,8,7,8);

			checkbutton = gtk_check_button_new_with_label ("保存聊天记录");
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton),TRUE);
			gtk_table_attach_defaults(GTK_TABLE (table), checkbutton, 0,8,8,9);

			label = gtk_label_new("提取消息，打开窗口热键");
			gtk_table_attach_defaults(GTK_TABLE (table), label, 0,4,9,10);

			label = gtk_label_new("老板键(隐藏显示所有窗口)");
			gtk_table_attach_defaults(GTK_TABLE (table), label, 4,8,9,10);

			entry = gtk_entry_new();
			gtk_entry_set_text(GTK_ENTRY(entry),"Ctrl+Alt+X");
			gtk_table_attach_defaults(GTK_TABLE (table), entry, 0,4,10,11);

			entry = gtk_entry_new();
			gtk_entry_set_text(GTK_ENTRY(entry),"无");
			gtk_table_attach_defaults(GTK_TABLE (table), entry, 4,8,10,11);

			label = gtk_label_new("最近联系人热键");
			gtk_table_attach_defaults(GTK_TABLE (table), label, 0,4,11,12);

			label = gtk_label_new("截图热键");
			gtk_table_attach_defaults(GTK_TABLE (table), label, 4,8,11,12);

			entry = gtk_entry_new();
			gtk_entry_set_text(GTK_ENTRY(entry),"无");
			gtk_table_attach_defaults(GTK_TABLE (table), entry, 0,4,12,13);

			entry = gtk_entry_new();
			gtk_entry_set_text(GTK_ENTRY(entry),"无");
			gtk_table_attach_defaults(GTK_TABLE (table), entry, 4,8,12,13);

			button = gtk_button_new_with_label("在线图标");;
			gtk_table_attach_defaults(GTK_TABLE (table), button, 0,2,13,14);

			entry = gtk_entry_new();
			gtk_table_attach_defaults(GTK_TABLE (table), entry, 2,4,13,14);

			button = gtk_button_new_with_label("消息图标");
			gtk_table_attach_defaults(GTK_TABLE (table), button, 4,6,13,14);

			entry = gtk_entry_new();
			gtk_table_attach_defaults(GTK_TABLE (table), entry, 6,8,13,14);




			frame = gtk_frame_new (t[i].text);
      			gtk_container_add(GTK_CONTAINER(frame),table);
			label = gtk_label_new (t[i].text);
       			gtk_notebook_append_page (GTK_NOTEBOOK (notebook), frame, label);
			}
			break;
		//发送接受设置
		case 2:{
			table = gtk_table_new(15,4,TRUE);
			checkbutton = gtk_check_button_new_with_label ("只提示消息，不自动弹出收到消息");
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton),TRUE);
			gtk_table_attach_defaults(GTK_TABLE (table), checkbutton, 0,4,0,1);

			checkbutton = gtk_check_button_new_with_label ("有要接收的文件时，自动弹出信息");
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton),TRUE);
			gtk_table_attach_defaults(GTK_TABLE (table), checkbutton, 0,4,1,2);

			checkbutton = gtk_check_button_new_with_label ("只提示不自动弹出消息时，以消息框提示信息内容");
			gtk_table_attach_defaults(GTK_TABLE (table), checkbutton, 0,4,2,3);

			checkbutton = gtk_check_button_new_with_label ("离线状态时不自动弹出收到信息");
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton),TRUE);
			gtk_table_attach_defaults(GTK_TABLE (table), checkbutton, 0,4,3,4);

			checkbutton = gtk_check_button_new_with_label ("离线时自动回复");
			gtk_table_attach_defaults(GTK_TABLE (table), checkbutton, 0,4,4,5);

			checkbutton = gtk_check_button_new_with_label ("收到消息时发出声音");
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton),TRUE);
			gtk_table_attach_defaults(GTK_TABLE (table), checkbutton, 0,4,5,6);

			checkbutton = gtk_check_button_new_with_label ("同一人聊天，按发送后同时关闭对话框");
			gtk_table_attach_defaults(GTK_TABLE (table), checkbutton, 0,4,6,7);

			checkbutton = gtk_check_button_new_with_label ("多人聊天时，按发送后同时关闭对话框");
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton),TRUE);
			gtk_table_attach_defaults(GTK_TABLE (table), checkbutton, 0,4,7,8);

			checkbutton = gtk_check_button_new_with_label ("关闭聊天对话框时清空接收框中内容");
			gtk_table_attach_defaults(GTK_TABLE (table), checkbutton, 0,4,8,9);

			checkbutton = gtk_check_button_new_with_label ("闪屏振动时，消息框提示");
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton),TRUE);
			gtk_table_attach_defaults(GTK_TABLE (table), checkbutton, 0,4,9,10);

			checkbutton = gtk_check_button_new_with_label ("对方打开消息时，进行通知(要求发送回执)");
			gtk_table_attach_defaults(GTK_TABLE (table), checkbutton, 0,4,10,11);

			checkbutton = gtk_check_button_new_with_label ("对方打开消息(发送回执)时，消息框提示");
			gtk_table_attach_defaults(GTK_TABLE (table), checkbutton, 0,4,11,12);

			checkbutton = gtk_check_button_new_with_label ("对方发送过来的消息需要回执时，发送已打开消息(回执)");
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton),TRUE);
			gtk_table_attach_defaults(GTK_TABLE (table), checkbutton, 0,4,12,13);

			checkbutton = gtk_check_button_new_with_label ("关闭对话框后自动释放对话框资源");
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton),TRUE);
			gtk_table_attach_defaults(GTK_TABLE (table), checkbutton, 0,4,13,14);

			checkbutton = gtk_check_button_new_with_label ("使用消息盒子(任务栏处以消息盒子显示未读信息)");
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton),TRUE);
			gtk_table_attach_defaults(GTK_TABLE (table), checkbutton, 0,4,14,15);

			frame = gtk_frame_new (t[i].text);
      			gtk_container_add(GTK_CONTAINER(frame),table);
			label = gtk_label_new (t[i].text);
       			gtk_notebook_append_page (GTK_NOTEBOOK (notebook), frame, label);
			}
			break;
		//群设置
		case 3:{
			table = gtk_table_new(10,4,TRUE);
			vbox1 = gtk_vbox_new(FALSE,0);
			frame = gtk_frame_new ("群消息默认设置");
			gtk_container_add(GTK_CONTAINER(frame),vbox1);
			radio = gtk_radio_button_new_with_label(NULL,"接收并提示信息");
			gtk_box_pack_start(GTK_BOX(vbox1), radio, FALSE, FALSE, 5);
			group = gtk_radio_button_get_group(GTK_RADIO_BUTTON(radio));

			radio = gtk_radio_button_new_with_label(group,"自动弹出消息");
			gtk_box_pack_start(GTK_BOX(vbox1), radio, FALSE, FALSE, 5);
			group = gtk_radio_button_get_group(GTK_RADIO_BUTTON(radio));

			radio = gtk_radio_button_new_with_label(group,"消息来时只显示消息数目");
			gtk_box_pack_start(GTK_BOX(vbox1), radio, FALSE, FALSE, 5);
			group = gtk_radio_button_get_group(GTK_RADIO_BUTTON(radio));

			radio = gtk_radio_button_new_with_label(group,"消息框提示信息内容");
			gtk_box_pack_start(GTK_BOX(vbox1), radio, FALSE, FALSE, 5);
			group = gtk_radio_button_get_group(GTK_RADIO_BUTTON(radio));

			radio = gtk_radio_button_new_with_label(group,"阻止一切该群的消息");
			gtk_box_pack_start(GTK_BOX(vbox1), radio, FALSE, FALSE, 5);
			group = gtk_radio_button_get_group(GTK_RADIO_BUTTON(radio));

			gtk_table_attach_defaults(GTK_TABLE (table), frame, 0,4,4,10);

			checkbutton = gtk_check_button_new_with_label ("不允许别人加我到群中");
			gtk_table_attach_defaults(GTK_TABLE (table), checkbutton, 0,4,0,1);

			checkbutton = gtk_check_button_new_with_label ("有用户加入群或退出群时，进行通知");
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton),TRUE);
			gtk_table_attach_defaults(GTK_TABLE (table), checkbutton, 0,4,1,2);

			checkbutton = gtk_check_button_new_with_label ("不接收来自群内的图片");
			gtk_table_attach_defaults(GTK_TABLE (table), checkbutton, 0,4,2,3);

			checkbutton = gtk_check_button_new_with_label ("收到消息时发出声音");
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton),TRUE);
			gtk_table_attach_defaults(GTK_TABLE (table), checkbutton, 0,4,3,4);


			frame = gtk_frame_new (t[i].text);
      			gtk_container_add(GTK_CONTAINER(frame),table);
			label = gtk_label_new (t[i].text);
       			gtk_notebook_append_page (GTK_NOTEBOOK (notebook), frame, label);
			}
			break;
		//功能设置
		case 4:{
			table = gtk_table_new(16,4,TRUE);
			checkbutton = gtk_check_button_new_with_label ("显示我的输入状态");
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton),TRUE);
			gtk_table_attach_defaults(GTK_TABLE (table), checkbutton, 0,4,0,1);

			checkbutton = gtk_check_button_new_with_label ("上线时通知其他用户");
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton),TRUE);
			gtk_table_attach_defaults(GTK_TABLE (table), checkbutton, 0,4,1,2);

			checkbutton = gtk_check_button_new_with_label ("下线时通知其他用户");
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton),TRUE);
			gtk_table_attach_defaults(GTK_TABLE (table), checkbutton, 0,4,2,3);

			checkbutton = gtk_check_button_new_with_label ("其他用户上线时，进行通知");
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton),TRUE);
			gtk_table_attach_defaults(GTK_TABLE (table), checkbutton, 0,4,3,4);

			checkbutton = gtk_check_button_new_with_label ("其他用户下线时，进行通知");
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton),TRUE);
			gtk_table_attach_defaults(GTK_TABLE (table), checkbutton, 0,4,4,5);

			checkbutton = gtk_check_button_new_with_label ("允许其他用户发出远程协助请求");
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton),TRUE);
			gtk_table_attach_defaults(GTK_TABLE (table), checkbutton, 0,4,5,6);

			checkbutton = gtk_check_button_new_with_label ("允许其他用户发出语音对话请求");
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton),TRUE);
			gtk_table_attach_defaults(GTK_TABLE (table), checkbutton, 0,4,6,7);

			checkbutton = gtk_check_button_new_with_label ("接收其他用户发出的表情和图片");
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton),TRUE);
			gtk_table_attach_defaults(GTK_TABLE (table), checkbutton, 0,4,7,8);

			checkbutton = gtk_check_button_new_with_label ("发送图片或表情时，压缩发送");
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton),TRUE);
			gtk_table_attach_defaults(GTK_TABLE (table), checkbutton, 0,4,8,9);

			checkbutton = gtk_check_button_new_with_label ("屏蔽垃圾信息");
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton),TRUE);
			gtk_table_attach_defaults(GTK_TABLE (table), checkbutton, 0,4,9,10);

			checkbutton = gtk_check_button_new_with_label ("允许其他用户发送闪屏振动");
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton),TRUE);
			gtk_table_attach_defaults(GTK_TABLE (table), checkbutton, 0,4,10,11);

			checkbutton = gtk_check_button_new_with_label ("允许自动接收对方发送过来的文件");
			gtk_table_attach_defaults(GTK_TABLE (table), checkbutton, 0,4,11,12);

			checkbutton = gtk_check_button_new_with_label ("不接收其他用户的所有消息(所有用户为黑名单)");
			gtk_table_attach_defaults(GTK_TABLE (table), checkbutton, 0,4,12,13);

			checkbutton = gtk_check_button_new_with_label ("主界面用户列表中不显示已设置成黑名单的用户");
			gtk_table_attach_defaults(GTK_TABLE (table), checkbutton, 0,4,13,14);

			checkbutton = gtk_check_button_new_with_label ("自动刷新");
			gtk_table_attach_defaults(GTK_TABLE (table), checkbutton, 0,4,14,15);

			checkbutton = gtk_check_button_new_with_label ("在线状态时状态自动转换");
			gtk_table_attach_defaults(GTK_TABLE (table), checkbutton, 0,4,15,16);

			frame = gtk_frame_new (t[i].text);
       			gtk_container_add(GTK_CONTAINER(frame),table);

			label = gtk_label_new (t[i].text);
       			gtk_notebook_append_page (GTK_NOTEBOOK (notebook), frame, label);
			}
			break;
		//网络设置
		case 5:{
			table = gtk_table_new(16,4,TRUE);
			frame = gtk_frame_new ("多网卡电脑请指定");
			//gtk_widget_modify_fg(frame,GTK__NORMAL,&blue);
			gtk_table_attach_defaults(GTK_TABLE (table),frame, 0,4,0,4);
			table1= gtk_table_new(2,6,TRUE);
			checkbutton = gtk_check_button_new_with_label ("指定IP地址");
			gtk_table_attach_defaults(GTK_TABLE (table1), checkbutton, 0,2,0,1);
			checkbutton = gtk_check_button_new_with_label ("指定MAC地址");
			gtk_table_attach_defaults(GTK_TABLE (table1), checkbutton, 0,2,1,2);
			entry = gtk_entry_new();
			gtk_entry_set_text(GTK_ENTRY(entry),"0 . 0 . 0 . 0");
			gtk_table_attach_defaults(GTK_TABLE (table1), entry, 2,4,0,1);
			entry = gtk_entry_new();
			gtk_table_attach_defaults(GTK_TABLE (table1), entry, 2,4,1,2);
			label = gtk_label_new("修改后下次有效");
			gtk_table_attach_defaults(GTK_TABLE (table1), label, 4,6,0,1);
			label = gtk_label_new("修改后下次有效");
			gtk_table_attach_defaults(GTK_TABLE (table1), label, 4,6,1,2);
			gtk_container_add(GTK_CONTAINER(frame),table1);

			frame = gtk_frame_new ("设置文件发送接收缓冲区大小(可用来限制文件传送速度)");
			gtk_table_attach_defaults(GTK_TABLE (table),frame, 0,4,4,8);
			table1= gtk_table_new(2,8,TRUE);
			label = gtk_label_new ("发送文件缓冲区大小:");
			gtk_table_attach_defaults(GTK_TABLE (table1), label, 0,4,0,1);
			label = gtk_label_new ("接收文件缓冲区大小:");
			gtk_table_attach_defaults(GTK_TABLE (table1), label, 0,4,1,2);
			entry = gtk_entry_new();
			gtk_entry_set_text(GTK_ENTRY(entry),"65536");
			gtk_table_attach_defaults(GTK_TABLE (table1), entry, 4,6,0,1);
			entry = gtk_entry_new();
			gtk_entry_set_text(GTK_ENTRY(entry),"65536");
			gtk_table_attach_defaults(GTK_TABLE (table1), entry, 4,6,1,2);
			label = gtk_label_new("(字节)");
			gtk_table_attach_defaults(GTK_TABLE (table1), label, 6,8,0,1);
			label = gtk_label_new("(字节)");
			gtk_table_attach_defaults(GTK_TABLE (table1), label, 6,8,1,2);
			gtk_container_add(GTK_CONTAINER(frame),table1);

			frame = gtk_frame_new ("设置图片发送参数");
			gtk_table_attach_defaults(GTK_TABLE (table),frame, 0,4,8,12);
			table1= gtk_table_new(2,8,TRUE);
			label = gtk_label_new("UDP发送图片时，图片分段发送大小:");
			gtk_table_attach_defaults(GTK_TABLE (table1), label, 0,5,0,1);
			label = gtk_label_new("重试次数:");
			gtk_table_attach_defaults(GTK_TABLE (table1), label, 0,2,1,2);
			entry = gtk_entry_new();
			gtk_entry_set_text(GTK_ENTRY(entry),"512");
			gtk_table_attach_defaults(GTK_TABLE (table1), entry, 5,7,0,1);
			entry = gtk_entry_new();
			gtk_entry_set_text(GTK_ENTRY(entry),"10");
			gtk_table_attach_defaults(GTK_TABLE (table1), entry, 2,4,1,2);
			label = gtk_label_new("(字节)");
			gtk_table_attach_defaults(GTK_TABLE (table1), label, 7,8,0,1);
			gtk_container_add(GTK_CONTAINER(frame),table1);

			frame = gtk_frame_new ("自定义端口和群聊组播地址---下次运行时才生效");
			gtk_table_attach_defaults(GTK_TABLE (table),frame, 0,4,12,16);
			table1= gtk_table_new(2,6,TRUE);
			label = gtk_label_new("自定义端口:");
			gtk_table_attach_defaults(GTK_TABLE (table1), label, 0,2,0,1);
			label = gtk_label_new("群聊组播地址:");
			gtk_table_attach_defaults(GTK_TABLE (table1), label, 0,2,1,2);
			entry = gtk_entry_new();
			gtk_entry_set_text(GTK_ENTRY(entry),"2425");
			gtk_table_attach_defaults(GTK_TABLE (table1), entry, 2,5,0,1);
			entry = gtk_entry_new();
			gtk_entry_set_text(GTK_ENTRY(entry),"226. 81. 9 . 8");
			gtk_table_attach_defaults(GTK_TABLE (table1), entry, 2,5,1,2);
			button = gtk_button_new_with_label("默认");
			gtk_table_attach_defaults(GTK_TABLE (table1), button, 5,6,0,1);
			button = gtk_button_new_with_label("默认");
			gtk_table_attach_defaults(GTK_TABLE (table1), button, 5,6,1,2);
			gtk_container_add(GTK_CONTAINER(frame),table1);

			frame = gtk_frame_new (t[i].text);
       			gtk_container_add(GTK_CONTAINER(frame),table);

			label = gtk_label_new (t[i].text);
       			gtk_notebook_append_page (GTK_NOTEBOOK (notebook), frame, label);
			}
			break;
		//个性设置
		case 6:{
			table = gtk_table_new(18,4,TRUE);
			checkbutton = gtk_check_button_new_with_label ("有备注名称时不显示主机名");
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton),TRUE);
			gtk_table_attach_defaults(GTK_TABLE (table), checkbutton, 0,4,0,1);

			checkbutton = gtk_check_button_new_with_label ("搜索时显示搜索栏");
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton),TRUE);
			gtk_table_attach_defaults(GTK_TABLE (table), checkbutton, 0,4,1,2);

			checkbutton = gtk_check_button_new_with_label ("主界面上显示论坛栏(下次运行有效)");
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton),TRUE);
			gtk_table_attach_defaults(GTK_TABLE (table), checkbutton, 0,4,2,3);

			checkbutton = gtk_check_button_new_with_label ("小图标时选择中显示大图标状态");
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton),TRUE);
			gtk_table_attach_defaults(GTK_TABLE (table), checkbutton, 0,4,3,4);

			checkbutton = gtk_check_button_new_with_label ("聊天对话框右上角显示web内容(天气预报)");
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton),TRUE);
			gtk_table_attach_defaults(GTK_TABLE (table), checkbutton, 0,4,4,5);

			checkbutton = gtk_check_button_new_with_label ("我的好友组下自动分组");
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton),TRUE);
			gtk_table_attach_defaults(GTK_TABLE (table), checkbutton, 0,4,5,6);

			checkbutton = gtk_check_button_new_with_label ("保存聊天对话框右侧窗口显示状态");
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton),TRUE);
			gtk_table_attach_defaults(GTK_TABLE (table), checkbutton, 0,4,6,7);

			checkbutton = gtk_check_button_new_with_label ("保存聊天对话框高度");
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton),TRUE);
			gtk_table_attach_defaults(GTK_TABLE (table), checkbutton, 0,4,7,8);

			checkbutton = gtk_check_button_new_with_label ("允许与自己进行远程协助和语音对话");
			gtk_table_attach_defaults(GTK_TABLE (table), checkbutton, 0,4,8,9);

			checkbutton = gtk_check_button_new_with_label ("有要接收的文件时，不显示对方的资料");
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton),TRUE);
			gtk_table_attach_defaults(GTK_TABLE (table), checkbutton, 0,4,9,10);

			checkbutton = gtk_check_button_new_with_label ("拖拽文件发送时，无需点发送按钮，直接发送");
			gtk_table_attach_defaults(GTK_TABLE (table), checkbutton, 0,4,10,11);

			checkbutton = gtk_check_button_new_with_label ("进入系统时，不提示未提醒的日程安排提醒");
			gtk_table_attach_defaults(GTK_TABLE (table), checkbutton, 0,4,11,12);

			checkbutton = gtk_check_button_new_with_label ("重绘主界面和聊天对话框标题栏(下次运行有效)");
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton),TRUE);
			gtk_table_attach_defaults(GTK_TABLE (table), checkbutton, 0,4,12,13);

			frame = gtk_frame_new ("信息提示延迟时间设置(单位:秒)");
			gtk_table_attach_defaults(GTK_TABLE (table),frame, 0,4,13,18);
			table1 = gtk_table_new(3,6,TRUE);
			gtk_container_add(GTK_CONTAINER(frame),table1);
			label = gtk_label_new ("上线通知提示");
			gtk_table_attach_defaults(GTK_TABLE (table1), label, 0,2,0,1);

			label = gtk_label_new ("下线通知提示");
			gtk_table_attach_defaults(GTK_TABLE (table1), label, 3,5,0,1);

			label = gtk_label_new ("消息打开通知");
			gtk_table_attach_defaults(GTK_TABLE (table1), label, 0,2,1,2);

			label = gtk_label_new ("闪屏振动通知");
			gtk_table_attach_defaults(GTK_TABLE (table1), label, 3,5,1,2);

			label = gtk_label_new ("自动接收文件");
			gtk_table_attach_defaults(GTK_TABLE (table1), label, 0,2,2,3);

			label = gtk_label_new ("下线通知提示");
			gtk_table_attach_defaults(GTK_TABLE (table1), label, 3,5,2,3);

			entry = gtk_entry_new();
			gtk_entry_set_text(GTK_ENTRY(entry),"4");
			gtk_table_attach_defaults(GTK_TABLE (table1), entry, 2,3,0,1);

			entry = gtk_entry_new();
			gtk_entry_set_text(GTK_ENTRY(entry),"4");
			gtk_table_attach_defaults(GTK_TABLE (table1), entry, 5,6,0,1);

			entry = gtk_entry_new();
			gtk_entry_set_text(GTK_ENTRY(entry),"4");
			gtk_table_attach_defaults(GTK_TABLE (table1), entry, 2,3,1,2);

			entry = gtk_entry_new();
			gtk_entry_set_text(GTK_ENTRY(entry),"4");
			gtk_table_attach_defaults(GTK_TABLE (table1), entry, 5,6,1,2);

			entry = gtk_entry_new();
			gtk_entry_set_text(GTK_ENTRY(entry),"4");
			gtk_table_attach_defaults(GTK_TABLE (table1), entry, 2,3,2,3);

			entry = gtk_entry_new();
			gtk_entry_set_text(GTK_ENTRY(entry),"4");
			gtk_table_attach_defaults(GTK_TABLE (table1), entry, 5,6,2,3);

			frame = gtk_frame_new (t[i].text);
       			gtk_container_add(GTK_CONTAINER(frame),table);

			label = gtk_label_new (t[i].text);
       			gtk_notebook_append_page (GTK_NOTEBOOK (notebook), frame, label);
			}
			break;
		}
    	}

	cancel_button = gtk_button_new_with_label("取消");
	gtk_widget_set_size_request(cancel_button, 70, 28);
 	g_signal_connect (G_OBJECT (cancel_button), "clicked",G_CALLBACK (set_soft_quit), soft_set);
	gtk_box_pack_end(GTK_BOX(hbox1), cancel_button, FALSE, FALSE, 10);

	apply_button = gtk_button_new_with_label("应用");
	gtk_widget_set_size_request(apply_button, 70, 28);

	ok_button = gtk_button_new_with_label("确定");
	gtk_widget_set_size_request(ok_button, 70, 28);

	g_signal_connect (G_OBJECT (apply_button), "clicked",G_CALLBACK (set_soft_apply), soft_set);
	g_signal_connect (G_OBJECT (ok_button), "clicked",G_CALLBACK (set_soft_ok), soft_set);

	gtk_box_pack_end(GTK_BOX(hbox1), apply_button, FALSE, FALSE, 10);
	gtk_box_pack_end(GTK_BOX(hbox1), ok_button, FALSE, FALSE, 10);

	gtk_box_pack_start(GTK_BOX(vbox), hbox, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(vbox), hbox1, FALSE, FALSE, 0);

   	g_signal_connect (G_OBJECT (soft_set), "destroy",G_CALLBACK (set_soft_quit), soft_set);
	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(view));
	g_signal_connect(selection, "changed", G_CALLBACK(on_change), notebook);

	gtk_widget_show_all(soft_set);
}



}
