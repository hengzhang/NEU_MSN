#ifndef M_USER_H
#define M_USER_H
#include <netinet/in.h>
#include "m_linpop.h"
#include "g_db.h"
typedef struct LinpopUser_T
{
  gchar *nick;
  gchar *machine;
  gchar *user;
  gchar *group;
  guint recv;
  guint send;
  gboolean dialup;
  struct sockaddr_in from;
} LinpopUser;

typedef int (*userFunc)(LinpopUser *user,void *parm);
/*void printUserList(void);*/
void initUserList(void);
unsigned int getUserNumber();
gboolean deleteUserByIP(struct sockaddr_in addr);
void addUser(gchar *userName,gchar *machine,gchar *nick,gchar *group,struct sockaddr_in addr);
gboolean getUserByIP(struct sockaddr_in addr,LinpopUser *user);
void setRecv(struct sockaddr_in addr,guint recv);
void setSend(struct sockaddr_in addr,guint send);
void setDialup(struct sockaddr_in addr,gboolean dialup);
void setNick(struct sockaddr_in addr,gchar *nick);
void setGroup(struct sockaddr_in addr,gchar *group);
GList *getGroupList(void);
GList *getGroupUserList(gchar *group);

int userListForeach(userFunc func,void *parm);
void freeUser(LinpopUser *user);
/*#ifndef __M_USER_
extern DB *userList;
#endif*/
#endif
