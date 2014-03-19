#ifndef I_CHAT_H
#define I_CHAT_H
#include <netinet/in.h>
#include <gtk/gtk.h>
#include "g_db.h"
typedef struct LinpopChat_T
{
  GtkWidget *mainWindow;
  GtkWidget *showTextView;
  GtkWidget *inputTextView;
  GtkWidget *showWindow;
  GtkWidget *inputWindow;
  GtkWidget *selfImage;
  GtkWidget *otherImage;
  GtkWidget *selfLabel;
  GtkWidget *otherLabel;
  gchar *msg;
  struct sockaddr_in from;
} LinpopChat;

typedef int (*chatFunc)(LinpopChat *chat,void *parm);
/*void printUserList(void);*/
void initChatList(void);
unsigned int getChatNumber();
gboolean deleteChatByIP(struct sockaddr_in addr);
void addChat(LinpopChat *win);
gboolean getChatByIP(struct sockaddr_in addr,LinpopChat *chat);
int chatListForeach(chatFunc func,void *parm);
#endif
