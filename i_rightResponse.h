/********************************************/
/*名称：i_rightResponse.h
/*描述：为列表项创建右键菜单及其响应
/*作者：张珩
/*日期：
/********************************************/

#include <gtk/gtk.h>

//右键点击,第一个参数是控件，第二个是事件
static gint right_button_press (GtkWidget * wid, GdkEvent * event);

//为控件添加右键菜单
void add_right_menu (GtkWidget * wid, GtkMenu * menu);

//右键响应
void right_response(gchar *string);

//创建右键菜单，参数是需要创建右键菜单的元素
GtkWidget* getMenu_right(GtkWidget *item,char* buf_item);


