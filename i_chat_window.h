#ifndef I_CHAT_WINDOW_H
#define I_CHAT_WINDOW_H
#include "i_chat.h"

#define NO_SHOW 0x0
#define SHOW  0x1

/*＊＊＊＊＊＊＊＊人物形象＊＊＊＊＊*/
GtkWidget *buttonMyFace ;
GtkWidget *buttonHiFace ;

void clearTextView(GtkWidget *textView);
void printMsgToTextView(GtkWidget *textView,gchar *user,gchar *content);
void getTextFromTextView(GtkWidget *inputTextView,gchar *buf,guint n);
void printEchoToTextView(GtkWidget *textView,gchar *user,gchar *content);
void printInfoToTextView(GtkWidget *textView,gchar *user,gchar *info);
void createChatWindow(char *ip,char *name,int mode);
void fileSelect(GtkWidget *button, gpointer data);
#endif
