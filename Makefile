SOURCES = i_chat_window.c i_chat.c i_utils.c g_db.c g_properties.c m_linpop.c m_broadcast.c m_msg.c m_readMsg.c m_sendMsg.c m_user.c test.c m_kanji.c i_chat_callbacks.c i_addFace.c i_calendar.c i_fontSelect.c i_main_window.c i_main_callbacks.c i_main_rmenu.c i_entry_IP.c i_fileChooseDialog.c i_reminder.c i_rightResponse.c i_search_button.c i_textview.c i_list.c i_main_stmenu.c i_main_cmenu.c i_setting.c fileSelect.c filetansfer.c 
OBJS = ${SOURCES:.c=.o}
CFLAGS = `pkg-config gtk+-2.0 --cflags`
LDADD = `pkg-config gtk+-2.0 --libs`
CC = gcc
PACKAGE = test

all : ${OBJS}
	${CC} -o ${PACKAGE} ${OBJS} ${LDADD}
.c.o:
	${CC} ${CFLAGS} -c $<
clean:
	rm *.o
# end

