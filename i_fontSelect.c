#ifndef __I_FONTSELECT_
#define __I_FONTSELECT_
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <time.h>
#include  <gdk/gdkkeysyms.h>
#include <assert.h>
#include "i_chat.h"
#include "i_fontSelect.h"

/*字体选择结构体*/
typedef struct fontSelectParm_T{
    GtkWidget *window;
    GtkWidget *showTextView;
    GtkWidget *inputTextView;
}fontSelectParm;

/**************************************************/
/*函数名：doFontSelect
/*功能描述：字体选择确认
/*日期：
/*参数：
         GtkWidget *button ：控件指针
         fontSelectParm *parm:
/*返回值：void
/*作者：
/***************************************************/
static void doFontSelect( GtkWidget *button, fontSelectParm *parm)
{

  GtkStyle *style;
  gchar *font;
  PangoFontDescription *font_desc;
 
  font = gtk_font_selection_dialog_get_font_name (GTK_FONT_SELECTION_DIALOG (parm->window));

  font_desc = pango_font_description_from_string (font);
	
  style = gtk_style_copy (gtk_widget_get_style (parm->inputTextView));

  style->font_desc = font_desc;

  gtk_widget_set_style (parm->inputTextView, style);
  gtk_widget_set_style (parm->showTextView, style);
  gtk_widget_destroy(GTK_OBJECT(parm->window));
  g_print("font_ok");
}

/**************************************************/
/*函数名：closeWindow
/*功能描述：关闭窗口
/*日期：
/*参数：
         GtkWidget *button ：控件指针
         gchar *ip:
/*返回值：void
/*作者：
/***************************************************/
static void closeWindow(GtkWidget *widget,gpointer parm){
    gtk_widget_destroy(widget);
    g_free(parm);
}

/**************************************************/
/*函数名：fontSelect
/*功能描述：选择字体
/*日期：
/*参数：
         GtkWidget *button ：控件指针
         gchar *ip:
/*返回值：void
/*作者：
/***************************************************/
void fontSelect( GtkWidget *button, gchar *ip)
{

    GtkWidget *window;
    gboolean result;
    LinpopChat chat;
    struct sockaddr_in addr;
    inet_aton(ip,&addr.sin_addr);
    fontSelectParm *parm = g_malloc(sizeof(*parm));
    window = gtk_font_selection_dialog_new ("Linpop v1.0 字体选择");
    gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_MOUSE);
    result = getChatByIP(addr,&chat);
    assert(result == TRUE);
    parm->window = window;
    parm->showTextView = chat.showTextView;
    parm->inputTextView = chat.inputTextView;
    g_signal_connect (G_OBJECT (GTK_FONT_SELECTION_DIALOG (window)->ok_button),
		      "clicked", G_CALLBACK (doFontSelect),
		      parm);

    g_signal_connect (G_OBJECT (window), "destroy",
	              G_CALLBACK (closeWindow),
		      parm);

    gtk_signal_connect_object(GTK_OBJECT (GTK_FONT_SELECTION_DIALOG (window)->cancel_button),
			      "clicked",
			      GTK_SIGNAL_FUNC(closeWindow), 
			      parm);

    gtk_widget_show (window);

    g_print("font");

}
#endif
