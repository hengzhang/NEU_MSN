/**************************************************/
/*文件名：i_addFace.c
/*文件描述：表情选择
/*时间
/*作者:
/***************************************************/


#ifndef __I_ADDFACE_
#define __I_ADDFACE_
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <time.h>
#include  <gdk/gdkkeysyms.h>
#include  <assert.h>
#include "i_chat.h"
#include "i_addFace.h"
#include "i_utils.h"

#define ANIMATION_MAX 255
#define FILE_NAME_MAX 255
#define ANIMATION_COUNT 51

/*图形按钮结构体*/
typedef struct faceButtonParm_T{
    char ip[16];
    char num;
    GtkWidget *icon;
}faceButtonParm;

static GdkPixbufAnimation *animationArray[ANIMATION_MAX];
static GtkWidget *createBbox(GtkWidget *icon,gchar *ip);
static GtkWidget *createFaceButton(GtkWidget *icon,gchar *ip,char num);
static void faceButtonDestroy(GtkWidget *button,faceButtonParm *parm);
static void faceButtonCallback( GtkWidget *button,gpointer parm);

/**************************************************/
/*函数名：loadAnimationArray
/*功能描述：加载图像数组
/*时间：
/*返回值：void
/*作者:
/***************************************************/
void loadAnimationArray(void){
    GtkWidget *image;
    int i;
    size_t size;
    char buf[FILE_NAME_MAX]; 
    bzero(buf,sizeof(buf));
    strcpy(buf,"face/");
    size = strlen(buf);
    for(i = 0 ;i < ANIMATION_COUNT;i++){
        numToStr(buf + size,i + 1);
	strcat(buf,".gif");
        image = gtk_image_new_from_file(buf);
	animationArray[i] = gtk_image_get_animation(image);
	
    }
    return ;
}

/**************************************************/
/*函数名：getImageByNum
/*功能描述：绘制图片按钮
/*时间
/*参数：
       char num: 编号
/*返回值：GtkWidget *
/*作者:
/***************************************************/
GtkWidget *getImageByNum(char num){
    GtkImage *image = NULL;
    GdkPixbufAnimation *animation = NULL;
    animation = animationArray[num - 1];
    image = gtk_image_new_from_animation(animation);
    return image;
}

/**************************************************/
/*函数名：faceButtonCallback
/*功能描述：绘制图片按钮
/*时间
/*参数：
       GtkWidget *button ：控件指针
       gpointer parm:
/*返回值：void
/*作者:
/***************************************************/
static void faceButtonCallback( GtkWidget *button,gpointer parm)
{
    faceButtonParm *args = (faceButtonParm *)parm;
    LinpopChat chat;
    struct sockaddr_in addr;
    size_t sizeText,sizeMsg;
    g_print("OK\n");
    gboolean result;
    gchar *text = NULL;
    gchar *s,*t;
    GtkTextBuffer *buffer = NULL;
    GtkTextIter start,end;
    GtkImage *image;
    GdkPixbufAnimation *animation = NULL;
    GtkTextChildAnchor *anchor = NULL;
    inet_aton(args->ip,&addr.sin_addr);
    result = getChatByIP(addr,&chat);
    assert(result == TRUE);
    buffer = gtk_text_view_get_buffer(chat.inputTextView);
    gtk_text_buffer_get_bounds(buffer,&start,&end);
    text = gtk_text_buffer_get_text(buffer,&start,&end,FALSE);
    g_print("new text is:%s\n",text);
  
    for(sizeMsg = 0,t = chat.msg; *t;t++){
        if(*t == '\\'){
	    if(*(t+1) == '\\'){
	        sizeMsg++;
	    }
	    t++;
	}else{
	    sizeMsg++;
	}
    }
    for(s = text + sizeMsg;*s;s++,t++){
        if(*s == '\\'){
	    *t++ = '\\';
	    *t = '\\'; 
	}else{
	    *t = *s;
	}
    }
    *t++ = '\\';
    *t++ = args->num;
    *t = '\0';
    g_print("new chat.msg is:%s,size is:%d\n",chat.msg,sizeMsg);
    animation = animationArray[args->num - 1];
    image = gtk_image_new_from_animation(animation);
    anchor = gtk_text_buffer_create_child_anchor(buffer,&end);
    gtk_text_view_add_child_at_anchor(chat.inputTextView,image,anchor);
    gtk_widget_show(image);
    gtk_widget_destroy(GTK_OBJECT(args->icon));
}
static void faceButtonDestroy(GtkWidget *button,faceButtonParm *parm){
    g_free(parm);
}

/**************************************************/
/*函数名：createFaceButton
/*功能描述：绘制图片按钮
/*时间
/*参数：
       GtkWidget *icon ：控件指针
       gchar *ip：
       char num :
/*返回值：void
/*作者:
/***************************************************/
static GtkWidget *createFaceButton(GtkWidget *icon,gchar *ip,char num)
{
	GtkWidget *image;
	GtkWidget *button;
        faceButtonParm *parm = g_malloc(sizeof(*parm));
	bzero(parm,sizeof(*parm));
	strcpy(parm->ip,ip);
	parm->num = num;
	parm->icon = icon;
	button = gtk_button_new ();
	image = gtk_image_new_from_animation(animationArray[num -1]);
 
	g_signal_connect (G_OBJECT (button), "clicked",G_CALLBACK (faceButtonCallback), parm);
	g_signal_connect(G_OBJECT(button),"destroy",faceButtonDestroy,parm);

	gtk_container_add (GTK_CONTAINER (button), image);
	gtk_container_set_border_width (GTK_CONTAINER (button), 0);
	return button;
}

/**************************************************/
/*函数名：create_Bbox
/*功能描述：创建表情显示窗口
/*时间
/*参数：
       GtkWidget *icon ：控件
       gchar *ip：
/*返回值：void
/*作者:
/***************************************************/
static GtkWidget *createBbox(GtkWidget *icon,gchar *ip)
{
	GtkWidget *vbox;
	GtkWidget *bbox;
	GtkWidget *button;
	int i;
	vbox = gtk_vbox_new (FALSE, 0);
	bbox = gtk_hbox_new (FALSE, 0);
        for(i = 0 ; i < ANIMATION_COUNT; i++){
	    if((i != 0)  &&  ((i%10) == 0)){
	         gtk_container_add (GTK_CONTAINER (vbox), bbox);
	         bbox = gtk_hbox_new (FALSE, 0);
	    }
	    button = createFaceButton(icon,ip,i+1);
	    gtk_box_pack_start(GTK_BOX(bbox),button,FALSE,FALSE,0);
	}
	if((--i % 10) != 0)
	    gtk_container_add(GTK_CONTAINER(vbox),bbox);
	return vbox;
}

/**************************************************/
/*函数名：addFace
/*功能描述：添加表情
/*时间
/*参数：
       GtkWidget *widget: 控件指针
       gpointer ip ： 
/*返回值：void
/*作者:
/***************************************************/
void addFace(GtkWidget *widget, gpointer ip)
{

	GtkWidget *vbox;
	GtkWidget *icon;
	icon = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW (icon),GTK_WIN_POS_CENTER);
        gtk_window_set_skip_taskbar_hint(GTK_WINDOW (icon),TRUE);
	gtk_window_set_title (GTK_WINDOW (icon), "表情");
	gtk_window_set_keep_above(GTK_WINDOW (icon),TRUE);

	g_signal_connect (G_OBJECT (icon), "destroy",G_CALLBACK (gtk_widget_destroy),icon);

        vbox = createBbox(icon,ip);
	gtk_container_add (GTK_CONTAINER (icon), vbox);
	gtk_widget_show_all (icon);
}
#endif
