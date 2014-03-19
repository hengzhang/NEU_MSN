#ifndef I_UTILS_H
#define I_UTILS_H

GtkWidget *createImageLabelBox(gchar *imageName, gchar *labelText);
GdkPixbuf *createPixbufByName(const gchar *fileName);
GtkWidget *createImageButton(gchar *fileName);
void getCurrentTime(char buf[],int size);
void getCurrentDay(char  buf[],int size);
void numToStr(char *buf,int num);
void showTimeoutMessage(const gchar *msg);
void trayIconActivated(GObject *trayIcon, gpointer window);
void copyData(int from, int to);
void die(char * message);
#endif
