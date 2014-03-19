#ifndef M_MSG_H
#define M_MSG_H
#include <gtk/gtk.h>
#include <netinet/in.h>
#include <string.h>
#include "g_properties.h"
typedef struct LinpopMessage_T{
  struct sockaddr_in from;
  gint fromlen;
  gchar *serial;
  gchar *user;
  gchar *machine;
  gchar *group;
  guint com, com2;
  gchar *appendix;
} LinpopMessage;
static const char *date_week[7] = {
	("Sun"), ("Mon"), ("Tue"), ("Wed"), ("Thu"), ("Fri"), ("Sat"),
};
static const char *date_month[12] = {
	("Jan"), ("Feb"), ("Mar"), ("Apr"), ("May"), ("Jun"), ("Jul"), ("Aug"), ("Sep"), ("Oct"), ("Nov"), ("Dec"),
};

void freeMsg(LinpopMessage *msg);
void printMsg(LinpopMessage *msg,char *buf);
#endif
