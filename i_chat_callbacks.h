#ifndef I_CHAT_CALLBACKS_H
#define I_CHAT_CALLBACKS_H
#include <gtk/gtk.h>
void closeChatWindow(GtkWidget *widget,gpointer ip);
void sendEchoToTextView(GtkWidget *widget,gpointer ip);
void capture(GtkWidget *button, gpointer ip);
void undo(GtkWidget *widget,gpointer ip);
void changeFace(GtkButton* button,GtkButton* window);
void createChatRecordWindow(GtkWidget *widget,gpointer ip);
#endif
