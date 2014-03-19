/********************************************/
/*名称：m_kanji.h
/*描述：各种字符编码格式转换
/*作者：张珩
/*日期：2011.6.28
/********************************************/

#ifndef KANJI_H
#define KANJI_H
#include <stddef.h>

#ifndef is1sjis
#define is1sjis(c) (((0x81<=(unsigned char)(c))&&((unsigned char)(c)<=0x9f))||((0xe0<=(unsigned char)(c))&&((unsigned char)(c)<=0xfc)))//______
#endif
#ifndef is1euc
#define is1euc(c) ((0x81<=(unsigned char)(c))&&((unsigned char)(c)<=0xfe))//______
#endif


/******************************************************/
/*	定义与类型转换相关的结构体MB：
	枚举类型的变量：
		MB_INITIAL：初始字符值
		MB_KI1：
		MB_KI2：______
		MB_INKANJI： 最终字符值
		MB_KO1：______
		MB_KO2：______
	hi：______
		
/******************************************************/
struct MB {
	enum {
		MB_INITIAL,
		MB_KI1,
		MB_KI2, 
		MB_INKANJI, 
		MB_KO1, 
		MB_KO2
	} st;
	int hi; 
};

#define MB_CLEAR(mb) ((mb)->st = MB_INITIAL, (mb)->hi = 0)//______
#define KI_1 '$'//______
#define KO_1 '('//______

#define isskana(c) (((unsigned char)(c))>=0xa0&&((unsigned char)(c))<0xe0)//______

extern int kanji_in; //______
extern int kanji_out; //______


extern int euc_to_sjis(struct MB *mb, int ch, int (*put)(int));
extern int jis_to_sjis(struct MB *mb, int ch, int (*put)(int));
extern int sjis_to_euc(struct MB *mb, int ch, int (*put)(int));
extern int sjis_to_jis(struct MB *mb, int ch, int (*put)(int));
extern char * stretos(char *dst, const char *src);
extern char * strstoe(char *dst, const char *src);
extern size_t strstoelen(const char *src);
extern char * strstoe2(char *dst, const char *src);

#endif 
