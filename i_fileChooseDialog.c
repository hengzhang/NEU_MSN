/************************************************************/
/*
	1、创建一个文件选择窗口输入框
	2、创建一个文件保存窗口输入框

作者：张珩
日期：
*/
/**********************************************************/

#include"i_fileChooseDialog.h"
#include"i_textview.h"


/****************************************************/
/*
函数名:		create_filechooserdialog
功能：		创建一个文件选择窗口
参数：		void
返回值:		当前控件

*/
/**************************************************/
GtkWidget*
create_filechooserdialog(void)
{
  GtkWidget *filechooserdialog1;
  GtkWidget *dialog_vbox1;
  GtkWidget *dialog_action_area1;
  GtkWidget *button2;
  GtkWidget *button3;

  filechooserdialog1 = gtk_file_chooser_dialog_new ("", NULL, GTK_FILE_CHOOSER_ACTION_OPEN, NULL);
  gtk_container_set_border_width (GTK_CONTAINER (filechooserdialog1), 5);
  gtk_window_set_type_hint (GTK_WINDOW (filechooserdialog1), GDK_WINDOW_TYPE_HINT_DIALOG);

  dialog_vbox1 = GTK_DIALOG (filechooserdialog1)->vbox;
  gtk_widget_show (dialog_vbox1);

  dialog_action_area1 = GTK_DIALOG (filechooserdialog1)->action_area;
  gtk_widget_show (dialog_action_area1);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area1), GTK_BUTTONBOX_END);

  button2 = gtk_button_new_from_stock ("gtk-cancel");
  gtk_widget_show (button2);
  gtk_dialog_add_action_widget (GTK_DIALOG (filechooserdialog1), button2, GTK_RESPONSE_CANCEL);
  GTK_WIDGET_SET_FLAGS (button2, GTK_CAN_DEFAULT);

  button3 = gtk_button_new_from_stock ("gtk-open");
  gtk_widget_show (button3);
  g_signal_connect(G_OBJECT(button3),"clicked",G_CALLBACK(displayTextView),"10.10.31.1");
  gtk_dialog_add_action_widget (GTK_DIALOG (filechooserdialog1), button3, GTK_RESPONSE_OK);
  GTK_WIDGET_SET_FLAGS (button3, GTK_CAN_DEFAULT);

  /* Store pointers to all widgets, for use by lookup_widget(). */
//  GLADE_HOOKUP_OBJECT_NO_REF (filechooserdialog1, filechooserdialog1, "filechooserdialog1");
//  GLADE_HOOKUP_OBJECT_NO_REF (filechooserdialog1, dialog_vbox1, "dialog_vbox1");
//  GLADE_HOOKUP_OBJECT_NO_REF (filechooserdialog1, dialog_action_area1, "dialog_action_area1");
//  GLADE_HOOKUP_OBJECT (filechooserdialog1, button2, "button2");
//  GLADE_HOOKUP_OBJECT (filechooserdialog1, button3, "button3");

  gtk_widget_grab_default (button3);
  gtk_widget_show (filechooserdialog1);
  return filechooserdialog1;
}


/****************************************************/
/*
函数名:		create_filesavedialog
参数：		void
返回值:		当前控件
功能：		创建一个文件保存对话框
*/
/**************************************************/
GtkWidget*
create_filesavedialog(void)
{
  GtkWidget *filechooserdialog1;
  GtkWidget *dialog_vbox1;
  GtkWidget *dialog_action_area1;
  GtkWidget *button2;
  GtkWidget *button3;

  filechooserdialog1 = gtk_file_chooser_dialog_new ("", NULL, GTK_FILE_CHOOSER_ACTION_SAVE, NULL);
  gtk_container_set_border_width (GTK_CONTAINER (filechooserdialog1), 5);
  gtk_window_set_type_hint (GTK_WINDOW (filechooserdialog1), GDK_WINDOW_TYPE_HINT_DIALOG);

  dialog_vbox1 = GTK_DIALOG (filechooserdialog1)->vbox;
  gtk_widget_show (dialog_vbox1);

  dialog_action_area1 = GTK_DIALOG (filechooserdialog1)->action_area;
  gtk_widget_show (dialog_action_area1);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area1), GTK_BUTTONBOX_END);

  button2 = gtk_button_new_from_stock ("gtk-cancel");
  gtk_widget_show (button2);
  gtk_dialog_add_action_widget (GTK_DIALOG (filechooserdialog1), button2, GTK_RESPONSE_CANCEL);
  GTK_WIDGET_SET_FLAGS (button2, GTK_CAN_DEFAULT);

  button3 = gtk_button_new_from_stock ("gtk-open");
  gtk_widget_show (button3);
  gtk_dialog_add_action_widget (GTK_DIALOG (filechooserdialog1), button3, GTK_RESPONSE_OK);
  GTK_WIDGET_SET_FLAGS (button3, GTK_CAN_DEFAULT);

  /* Store pointers to all widgets, for use by lookup_widget(). */
//  GLADE_HOOKUP_OBJECT_NO_REF (filechooserdialog1, filechooserdialog1, "filechooserdialog1");
//  GLADE_HOOKUP_OBJECT_NO_REF (filechooserdialog1, dialog_vbox1, "dialog_vbox1");
//  GLADE_HOOKUP_OBJECT_NO_REF (filechooserdialog1, dialog_action_area1, "dialog_action_area1");
//  GLADE_HOOKUP_OBJECT (filechooserdialog1, button2, "button2");
//  GLADE_HOOKUP_OBJECT (filechooserdialog1, button3, "button3");

  gtk_widget_grab_default (button3);
  gtk_widget_show (filechooserdialog1);
  return filechooserdialog1;
}
