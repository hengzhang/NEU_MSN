#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>
#include <time.h>
#include <gdk/gdkkeysyms.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include "m_linpop.h"
#include "m_user.h"

#include "chatWindow.h"
//#include "linpop.h"
#include "m_sendMessage.h"

/**************************************************/
/*Ãû³Æ£ºsendToTextView1
/*ÃèÊö£º·¢ËÍÏûÏ¢»Øµ÷º¯Êý
/*×÷³ÉÈÕÆÚ£º 2010-07-01
/*²ÎÊý£º
         ²ÎÊý1£ºbutton¡¢GtkWidget*¡¢Á¬œÓ»Øµ÷º¯ÊýµÄ¿ØŒþ£¬·¢ËÍ°ŽÅ¥
         ²ÎÊý2£ºbuffer¡¢GtkTextBuffer¡¢»Øµ÷º¯ÊýŽ«ÈëµÄ²ÎÊý£¬ÎªœÓÊÕÏûÏ¢Ž°ÌåµÄ»º³åÇøÓò
/*·µ»ØÖµ£ºvoid
/*×÷Õß£ºÁõŸ°Ã÷¡ª¡ªteam5
/***************************************************/
/*œ«·¢ËÍÎÄ±Ÿ¿òµÄÎÄ±Ÿ·¢ËÍµœœÓÊÕÎÄ±Ÿ¿ò*/


//add by wanglong socket 
extern int s;
void getSystemTime(char buf[])
{
	time_t timep;
	struct tm * p;
	time(&timep);
	p = localtime(&timep);
	strftime(buf,30,"%H:%M:%S",p);
	//printf("%s\n",buf);
}
void sendToTextView1(GtkWidget *button, textView *textViewAll)
{
        GtkTextIter start,end;
	
	char *text ;
        gboolean result;
        char *ip;
	char *name;
	//strcpy(text,"\0");
	ip = textViewAll->ip;
	gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(textViewAll->buffer2),&start,&end);
	text=gtk_text_buffer_get_text(GTK_TEXT_BUFFER(textViewAll->buffer2),&start,&end,FALSE);

	if(strcmp(textViewAll->message,"\0")!=0)
	{
		strcat(text,"#");
		strcat(text,textViewAll->message);
g_print("%s\n","textViewAll->message != NULL");
	}

	g_print("%s\n","SEND1");
        g_print("%s\n",text);

textViewAll->message = "\0";
//add by wanglong  send message start
	struct sockaddr_in addr;
	bzero(&addr,sizeof(struct sockaddr_in));

	addr.sin_family=AF_INET;
	addr.sin_port=htons(LINPOP_DEFAULT_PORT);
	inet_pton(AF_INET,ip,&addr.sin_addr.s_addr);
        LinpopUser *user = g_malloc(sizeof(*user));
        result = getUserByIP(addr,user);
        if(result == FALSE){
              g_print("No this User\n");
              g_free(user);
        }
        else{
             LinpopSend(&user->from,LINPOP_SENDMSG,text);
             freeUser(user);
	}
	//if(msg_send(LINPOP_SENDMSG,text,(struct sockaddr *)&addr,s) != 0)
	//	die("send error!");
//add by wanglong send message end 

	showMessage(text,textViewAll);
	gtk_text_buffer_delete(GTK_TEXT_BUFFER(textViewAll->buffer2),&start,&end);
}
