/************************************************************/
/*
	1、创建一个提示窗口
	

作者：张珩
日期：
*/
/**********************************************************/
#include"i_reminder.h"

/****************************************************/
/*
函数名:		create_reminder_window
功能：		创建提示窗口
参数：		void
返回值:		当前控件

*/
/**************************************************/
GtkWidget* create_reminder_window (void)
{
  GtkWidget *dialog1;
  GtkWidget *dialog_vbox1;
  GtkWidget *label1;
  GtkWidget *dialog_action_area1;
  //GtkWidget *okbutton1;

  dialog1 = gtk_dialog_new ();
  gtk_window_set_title (GTK_WINDOW (dialog1), "提示");
  gtk_window_set_type_hint (GTK_WINDOW (dialog1), GDK_WINDOW_TYPE_HINT_DIALOG);

  dialog_vbox1 = GTK_DIALOG (dialog1)->vbox;
  gtk_widget_show (dialog_vbox1);

  label1 = gtk_label_new ("无此IP的聊天记录，请检查IP正确性！");
  gtk_widget_show (label1);
  gtk_box_pack_start (GTK_BOX (dialog_vbox1), label1, FALSE, FALSE, 0);
  gtk_widget_set_size_request (label1, -1, 130);

  dialog_action_area1 = GTK_DIALOG (dialog1)->action_area;
  gtk_widget_show (dialog_action_area1);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area1), GTK_BUTTONBOX_END);

/*  okbutton1 = gtk_button_new_from_stock ("gtk-ok");
  gtk_widget_show (okbutton1);
  gtk_dialog_add_action_widget (GTK_DIALOG (dialog1), okbutton1, GTK_RESPONSE_OK);
  GTK_WIDGET_SET_FLAGS (okbutton1, GTK_CAN_DEFAULT);
  g_signal_connect(G_OBJECT(okbutton1),"clicked",G_CALLBACK(gtk_widget_destroy),NULL);*/
  return dialog1;
}
