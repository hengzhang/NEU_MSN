#include "g_db.h"
#include "m_linpop.h"
#include "m_kanji.h"
#include "i_chat.h"
#include "i_addFace.h"
#include "i_main_window.h"
#include "filetansfer.h"
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <glib.h>

static void (*at_exit_proc)(void);
static void exit_proc(void)
{
	LinpopQuit();
}
static void kill_exit_proc(int dummy)
{
	alarm(0);
	exit(EXIT_SUCCESS);
}
static void trap_usr1(int dummy)
{
	//LinpopOpenSendWin();
    LinpopSendBroadcast(LINPOP_BR_ENTRY,"Hello");
}


int main(int argc,char *argv[])
{
    gtk_init(&argc,&argv);
    LinpopSetup(LINPOP_DEFAULT_PORT);
    initChatList();
    loadAnimationArray();
    LinpopLogin();
    createMainWindow();
    g_print("OK\n");
	signal(SIGINT, trap_usr1);
    signal(SIGTSTP,exit_proc);
	signal(SIGTERM, kill_exit_proc);
	atexit(exit_proc);
    gtk_main();

    //GMainLoop *loop = g_main_loop_new(g_main_context_new(),FALSE);
    //g_main_loop_run(loop);
    return 0;
}
