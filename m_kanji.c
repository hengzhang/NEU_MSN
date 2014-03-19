/************************************************************/
/*
	1、定义了与字符编码转换相关的函数
		函数：
		a、 ______——static int etoj(int wc);
		b、 ______static int etos(int wc);
		c、 ______static int jtoe(int wc);
		d、 ______static int jtos(int wc);
		e、 ______static int stoe(int wc);
		f、 ______static int stoj(int wc);
		g、 ______static int get_euc(struct MB *mb, int ch, int (*put)(int));
		h、 ______static int get_sjis(struct MB *mb, int ch, int (*put)(int));
		i、 ______static int get_jis(struct MB *mb, int ch, int (*put)(int));
		j、 ______——int euc_to_sjis(struct MB *mb, int ch, int (*put)(int))
		k、 ______——int jis_to_sjis(struct MB *mb, int ch, int (*put)(int))
		l、 ______——int sjis_to_euc(struct MB *mb, int ch, int (*put)(int))
		m、 ______——int sjis_to_jis(struct MB *mb, int ch, int (*put)(int))
		n、______—— char * stretos(char *dst, const char *src)
		o、 ______——char * strstoe(char *dst, const char *src)
		p、 ______——size_t strstoelen(const char *src)
		q、 ______——char * strstoe2(char *dst, const char *src)

/*作者：张珩
/*日期：2011.6.28
/**********************************************************/

/*
 * kanji.c - kanji code converter
 * Copyright (C) 1994, 1996 by candy
 */
const char rcsid_kanji[] = "$Id: kanji.c,v 1.1.1.2 1999/09/27 06:42:46 matty Exp $";
#include <stdio.h>
#include <stddef.h>
#include "m_kanji.h"

int kanji_in = '@';
int kanji_out = 'J';

static int etoj(int wc);
static int etos(int wc);
static int jtoe(int wc);
static int jtos(int wc);
static int stoe(int wc);
static int stoj(int wc);
static int get_euc(struct MB *mb, int ch, int (*put)(int));
static int get_sjis(struct MB *mb, int ch, int (*put)(int));
static int get_jis(struct MB *mb, int ch, int (*put)(int));

/*
 * EUC €ò JIS 0x2121..0x7e7e 
 * 0x00 .. 0xff €Ï€œ€Î€Þ€Þ¡£
 */
/****************************************************/
/*
函数名:		etoj
功能：		______
参数：		int wc：______
返回值:		static int ：______
*/
/**************************************************/
static int
etoj(int wc)
{
	int ret, hib = (wc >> 8) & 0x7f, lob = wc & 0x7f;
	if (hib != 0)
		ret = (hib << 8) | lob;
	else
		ret = wc;
	return ret;
}/* etoj */

/*
 * EUC €ò shift-JIS 
 * 0x00 .. 0xff €Ï€œ€Î€Þ€Þ¡£
 */
/****************************************************/
/*
函数名:		etos
功能：		______
参数：		int wc：______
返回值:		static int ：______
*/
/**************************************************/
static int
etos(int wc)
{
	int ret, hib = (wc >> 8) & 0x7f, lob = wc & 0x7f;
	if ((wc & 0xff00) == 0x8e00) {
		ret = wc & 0xff;
	}
	else if (hib != 0)
		ret = jtos((hib << 8) | lob);
	else
		ret = wc;
	return ret;
}/* etos */

/*
 * JIS 0x2121..0x7e7e €ò EUC 
 * 0x00 .. 0xff €Ï€œ€Î€Þ€Þ¡£
 */
/****************************************************/
/*
函数名:		jtoe
功能：		______
参数：		int wc：______
返回值:		static int ：______
*/
/**************************************************/
static int
jtoe(int wc)
{
	int ret, hib = (wc >> 8) & 0xff, lob = wc & 0xff;
	if (hib != 0)
		ret = ((hib | 0x80) << 8) | (lob | 0x80);
	else
		ret = wc;
	return ret;
}/* jtoe */

/*
 * JIS 0x2121..0x7e7e €ò shift-JIS 
 * 0x00 .. 0xff €Ï€œ€Î€Þ€Þ¡£
 */
/****************************************************/
/*
函数名:		jtos
功能：		______
参数：		int wc：______
返回值:		static int ：______
*/
/**************************************************/
static int
jtos(int wc)
{
	int ret = 0, err = 0;
	int hib = (wc >> 8) & 0xff, lob = wc & 0xff, hi1 = 0x81, hi2 = 0x21, lo1 = 0x1f;
	if (hib >= 0x21 && hib < 0x5f)
		hi1 = 0x81;
	else {
		if (hib >= 0x5f && hib < 0x80)
			hi1 = 0xc1;
		else
			err = -1;
	}
	if (err == 0) {
		if ((hib & 1) == 1) {
			hi2 = 0x21;
			if (lob >= 0x21 && lob < 0x60)
				lo1 = 0x1f;
			else {
				if (lob >= 0x60 && lob < 0x7f)
					lo1 = 0x20;
				else
					err = -1;
			}
		}
		else {
			hi2 = 0x22;
			if (lob >= 0x21 && lob < 0x7f)
				lo1 = 0x7e;
			else
				err = -1;
		}
	}
	if (err == 0)
		ret = (((hib - hi2) / 2 + hi1) << 8) + lob + lo1;
	else
		ret = wc;
	return ret;
}/* jtos */

/*
 * shift-JIS €ò EUC 
 * 0x00 .. 0xff €Ï€œ€Î€Þ€Þ¡£
 */
/****************************************************/
/*
函数名:		stoe
功能：		______
参数：		int wc：______
返回值:		static int ：______
*/
/**************************************************/
static int
stoe(int wc)
{
	int ret, hib = (wc >> 8) & 0xff;
	if (hib != 0)
		ret = stoj(wc) | 0x8080;
	else
		ret = wc;
	return ret;
}/* stoe */

/*
 * shift-JIS €ò JIS 0x2121..0x7e7e 
 * 0x00 .. 0xff €Ï€œ€Î€Þ€Þ¡£
 */
/****************************************************/
/*
函数名:		stoj
功能：		______
参数：		int wc：______
返回值:		static int ：______
*/
/**************************************************/
static int
stoj(int wc)
{
	int err = 0, ret = 0;
	int hib = (wc >> 8) & 0xff, lob = wc & 0xff, hi1 = 0x81, hi2 = 0x21, lo1 = 0x1f;
	if (hib >= 0x81 && hib < 0xa0)
		hi1 = 0x81;
	else {
		if (hib >= 0xe0 && hib < 0xf0)
			hi1 = 0xc1;
		else
			err = -1;
	}
	if (err == 0) {
		if (lob >= 0x40 && lob < 0x7f) {
			hi2 = 0x21;
			lo1 = 0x1f;
		}
		else {
			if (lob >= 0x80 && lob < 0x9f) {
				hi2 = 0x21;
				lo1 = 0x20;
			}
			else {
				if (lob >= 0x9f && lob < 0xfd) {
					hi2 = 0x22;
					lo1 = 0x7e;
				}
				else
					err = -1;
			}
		}
	}
	if (err == 0)
		ret = (((hib - hi1) * 2 + hi2) << 8) + lob - lo1;
	else
		ret = wc;
	return ret;
}/* stoj */

/*
 * ch == 0 €ÇžÆ€ÓœÐ€¹€È¡¢*mb €òœéŽü²œ€¹€ë¡£
 * ch == EOF €ÇžÆ€ÓœÐ€¹€È¡¢¥×¡Œ¥ë€·€Æ€¢€ëÊž»ú(033 €Ê€É)€òÁÝ€­œÐ€¹¡£
 * Ìá€êÃÍ€Ï¡¢Êž»ú¥³¡Œ¥É(single-byte: 1..0xff, multi-byte: 0x8181..0xfefe)
 * 0 €Ê€é€Ð¡¢Êž»ú€Ï€Ê€€(¥š¥¹¥±¡Œ¥×¥·¡Œ¥±¥ó¥¹œèÍýÃæ€Ê€É)
 */
/****************************************************/
/*
函数名:		get_euc
功能：		______
参数：		struct MB *mb：______, int ch：______, int (*put)(int)：______
返回值:		static int ：______
*/
/**************************************************/
static int
get_euc(struct MB *mb, int ch, int (*put)(int))
{
	int ret = 0;
	if (ch == 0) {
		MB_CLEAR(mb);
	}
	else {
		if (ch == EOF) {
			if (mb->hi)
				put(mb->hi);
			mb->hi = 0;
		}
		else if (mb->hi) {
			ret = (mb->hi << 8) + ch;
			mb->hi = 0;
		}
		else {
			if (ch >= 0x80)
				mb->hi = ch;
			else
				ret = ch;
		}
	}
	return ret;
}/* get_euc */

/*
 * ch == 0 €ÇžÆ€ÓœÐ€¹€È¡¢*mb €òœéŽü²œ€¹€ë¡£
 * ch == EOF €ÇžÆ€ÓœÐ€¹€È¡¢¥×¡Œ¥ë€·€Æ€¢€ëÊž»ú(033 €Ê€É)€òÁÝ€­œÐ€¹¡£
 * Ìá€êÃÍ€Ï¡¢Êž»ú¥³¡Œ¥É(single-byte: 1..0xff, multi-byte: 0x8181..0xfefe)
 * 0 €Ê€é€Ð¡¢Êž»ú€Ï€Ê€€(¥š¥¹¥±¡Œ¥×¥·¡Œ¥±¥ó¥¹œèÍýÃæ€Ê€É)
 */
/****************************************************/
/*
函数名:		get_sjis
功能：		______
参数：		struct MB *mb：______, int ch：______, int (*put)(int)：______
返回值:		static int ：______
*/
/**************************************************/
static int
get_sjis(struct MB *mb, int ch, int (*put)(int))
{
	int ret = 0;
	if (ch == 0) {
		MB_CLEAR(mb);
	}
	else {
		if (ch == EOF) {
			if (mb->hi)
				put(mb->hi);
			mb->hi = 0;
		}
		else if (mb->hi) {
			ret = (mb->hi << 8) + ch;
			mb->hi = 0;
		}
		else {
			if (is1sjis(ch))
				mb->hi = ch;
			else
				ret = ch;
		}
	}
	return ret;
}/* get_sjis */

/*
 * ch == 0 €ÇžÆ€ÓœÐ€¹€È¡¢*mb €òœéŽü²œ€¹€ë¡£
 * ch == EOF €ÇžÆ€ÓœÐ€¹€È¡¢¥×¡Œ¥ë€·€Æ€¢€ëÊž»ú(033 €Ê€É)€òÁÝ€­œÐ€¹¡£
 * Ìá€êÃÍ€Ï¡¢Êž»ú¥³¡Œ¥É(single-byte: 1..0xff, multi-byte: 0x2121..0xfefe)
 * 0 €Ê€é€Ð¡¢Êž»ú€Ï€Ê€€(¥š¥¹¥±¡Œ¥×¥·¡Œ¥±¥ó¥¹œèÍýÃæ€Ê€É)
 */
/****************************************************/
/*
函数名:		get_jis
功能：		______
参数：		struct MB *mb：______, int ch：______, int (*put)(int)：______
返回值:		static int ：______
*/
/**************************************************/
static int
get_jis(struct MB *mb, int ch, int (*put)(int))
{
	int ret = 0;
	if (ch == 0) {
		MB_CLEAR(mb);
	}
	else if (ch == '\n') {
		return ch;
	}
	else {
		switch (mb->st) {
		case MB_KI1:
			if (ch == KI_1) {
				mb->st = MB_KI2;
			}
			else {
				mb->st = MB_INITIAL;
				put(033);
			}
			break;
		case MB_KI2:
			if (ch == kanji_in || ch == '@' || ch == 'B') {
				mb->st = MB_INKANJI;
				ch = 0;
			}
			else {etoj
				mb->st = MB_INITIAL;
				put(033);
				put(KI_1);
			}
			break;
		case MB_KO1:
			if (ch == KO_1) {
				mb->st = MB_KO2;
			}
			else {
				mb->st = MB_INKANJI;
				put(033);
			}
			break;
		case MB_KO2:
			if (ch == kanji_out || ch == 'J' || ch == 'H' || ch == 'B') {
				mb->st = MB_INITIAL;
				ch = 0;
			}
			else {
				mb->st = MB_INKANJI;
				put(033);
				put(KO_1);
			}
			break;
		default:
			break;
		}/* switch */
		switch (mb->st) {
		case MB_INITIAL:
			if (ch == EOF)
				ret = 0;
			else if (ch == 033)
				mb->st = MB_KI1;
			else
				ret = ch;
			break;
		case MB_INKANJI:
			if (ch == EOF) {
				if (mb->hi) {
					mb->st = MB_INITIAL;
					put(mb->hi);
					mb->hi = 0;
				}
			}
			else if (ch == 033)
				mb->st = MB_KO1;
			else {
				if (mb->hi) {
					ret = (mb->hi << 8) + ch;
					mb->hi = 0;
				}
				else
					mb->hi = ch;
			}
			break;
		default:
			break;
		}/* switch */
	}
	return ret;
}/* get_jis */

/*
 * EUC Êž»úÎó€ò shift-JIS €ËÊÑŽ¹€¹€ë¡£
 * ch == 0 €Ç€Þ€ºžÆ€ÓœÐ€·¡¢Êž»úÎó€«€é£±¥Ð¥€¥È€º€Ä ch €ËÆþ€ì€ÆžÆ€ÓœÐ€·¡¢
 * ºÇžå€Ï ch == EOF €ÇžÆ€ÓœÐ€¹¡£
 * œÐÎÏ€Ï put() €òÄÌ€ž€Æ¹Ô€ï€ì€ë¡£
 */
/****************************************************/
/*
函数名:		euc_to_sjis
功能：		______
参数：		struct MB *mb：______, int ch：______, int (*put)(int)：______
返回值:		int ：______
*/
/**************************************************/
int
euc_to_sjis(struct MB *mb, int ch, int (*put)(int))
{
	int wc = get_euc(mb, ch, put);
	if (wc != 0) {
		wc = etos(wc);
		if (wc != 0) {
			if (wc & 0xff00) {
				put((wc >> 8) & 0xff);
			}
			put(wc & 0xff);
		}
	}
	return wc;
}/* euc_to_sjis */

/*
 * JIS Êž»úÎó€ò shift-JIS €ËÊÑŽ¹€¹€ë¡£
 * ch == 0 €Ç€Þ€ºžÆ€ÓœÐ€·¡¢Êž»úÎó€«€é£±¥Ð¥€¥È€º€Ä ch €ËÆþ€ì€ÆžÆ€ÓœÐ€·¡¢
 * ºÇžå€Ï ch == EOF €ÇžÆ€ÓœÐ€¹¡£
 * œÐÎÏ€Ï put() €òÄÌ€ž€Æ¹Ô€ï€ì€ë¡£
 */
/****************************************************/
/*
函数名:		jis_to_sjis
功能：		______
参数：		struct MB *mb：______, int ch：______, int (*put)(int)：______
返回值:		int ：______
*/
/**************************************************/
int
jis_to_sjis(struct MB *mb, int ch, int (*put)(int))
{
	int wc = get_jis(mb, ch, put);
	if (wc != 0) {
		wc = jtos(wc);
		if (wc != 0) {
			if (wc & 0xff00) {
				put((wc >> 8) & 0xff);
			}
			put(wc & 0xff);
		}
	}
	return wc;
}/* jis_to_sjis */

/*
 */

/****************************************************/
/*
函数名:		sjis_to_euc
功能：		______
参数：		struct MB *mb：______, int ch：______, int (*put)(int)：______
返回值:		int ：______
*/
/**************************************************/
int
sjis_to_euc(struct MB *mb, int ch, int (*put)(int))
{
	int wc = get_sjis(mb, ch, put);
	if (wc != 0) {
		wc = stoe(wc);
		if (wc != 0) {
			if (wc & 0xff00) {
				put((wc >> 8) & 0xff);
			}
			put(wc & 0xff);
		}
	}
	return wc;
}/* sjis_to_euc */

/*
 */
/****************************************************/
/*
函数名:		sjis_to_jis
功能：		______
参数：		struct MB *mb：______, int ch：______, int (*put)(int)：______
返回值:		int ：______
*/
/**************************************************/
int
sjis_to_jis(struct MB *mb, int ch, int (*put)(int))
{
	int wc = get_sjis(mb, ch, put);
	static int inkanji = 0;
	if (wc != 0) {
		wc = stoj(wc);
		if (wc != 0) {
			if (wc & 0xff00) {
				if (!inkanji) {
					inkanji = 1;
					put('\033');
					put(KI_1);
					put(kanji_in);
				}
				put((wc >> 8) & 0xff);
			}
			else {
				if (inkanji) {
					inkanji = 0;
					put('\033');
					put(KO_1);
					put(kanji_out);
				}
			}
			put(wc & 0xff);
		}
	}
	return wc;
}/* sjis_to_jis */

#define GETWCH(s) (((unsigned char)(s)[0] << 8) | (unsigned char)(s)[1])

/*
 *
 */
/****************************************************/
/*
函数名:		stretos
功能：		______
参数：		char *dst：______, const char *src：______
返回值:		char ：______
*/
/**************************************************/
char *
stretos(char *buf, const char *src)
{
	const unsigned char *s = (const unsigned char *)src;
	char *d = buf;
	while (*s != '\0') {
		int wc;
		if (is1euc(*s)) {
			wc = GETWCH(s);
			wc = etos(wc);
			if (s[1] != '\0')
				s++;
		}
		else {
			wc = *s;
		}
		if ((wc & ~0xff) != 0) {
			*d++ = wc >> 8;
		}
		*d++ = wc;
		s++;
	}/* while */
	*d = '\0';
	return buf;
}/* stretos */

/*
 * buf == src €À€È€À€á(¥«¥Ê€Î»þ)¡£
 */
/****************************************************/
/*
函数名:		strstoe
功能：		______
参数：		char *dst：______, const char *src：______
返回值:		char ：______
*/
/**************************************************/
char *
strstoe(char *buf, const char *src)
{
	const unsigned char *s = (const unsigned char *)src;
	char *d = buf;
	while (*s != '\0') {
		int wc;
		if (is1sjis(*s)) {
			wc = GETWCH(s);
			wc = stoe(wc);
			if (s[1] != '\0')
				s++;
		}
		else if (isskana(*s)) {
			wc = 0x8e00 | *s;
		}
		else {
			wc = *s;
		}
		if ((wc & ~0xff) != 0) {
			*d++ = wc >> 8;
		}
		*d++ = wc;
		s++;
	}/* while */
	*d = '\0';
	return buf;
}/* strstoe */

/*
 * strstoe() €·€¿»þ€Î¥Ð¥€¥È¿ô€òÊÖ€¹¡£
 */
/****************************************************/
/*
函数名:		strstoe
功能：		______
参数：		const char *src：______
返回值:		size_t ：______
*/
/**************************************************/
size_t
strstoelen(const char *src)
{
	size_t d = 0;
	const unsigned char *s = (const unsigned char *)src;
	while (*s != '\0') {
		int wc;
		if (is1sjis(*s)) {
			wc = GETWCH(s);
			wc = stoe(wc);
			if (s[1] != '\0')
				s++;
		}
		else if (isskana(*s)) {
			wc = 0x8e00 | *s;
		}
		else {
			wc = *s;
		}
		if ((wc & ~0xff) != 0) {
			d++;
		}
		d++;
		s++;
	}/* while */
	return d;
}/* strstoelen */

static unsigned short kana_zen[64] = {
	0xa1a1, 0xa1a3, 0xa1d6, 0xa1d7, 0xa1a2, 0xa1a6, 0xa5f2, 0xa5a1, 
	0xa5a3, 0xa5a5, 0xa5a7, 0xa5a9, 0xa5e3, 0xa5e5, 0xa5e7, 0xa5c3, 
	0xa1bc, 0xa5a2, 0xa5a4, 0xa5a6, 0xa5a8, 0xa5aa, 0xa5ab, 0xa5ad, 
	0xa5af, 0xa5b1, 0xa5b3, 0xa5b5, 0xa5b7, 0xa5b9, 0xa5bb, 0xa5bd, 
	0xa5bf, 0xa5c1, 0xa5c4, 0xa5c6, 0xa5c8, 0xa5ca, 0xa5cb, 0xa5cc, 
	0xa5cd, 0xa5ce, 0xa5cf, 0xa5d2, 0xa5d5, 0xa5d8, 0xa5db, 0xa5de, 
	0xa5df, 0xa5e0, 0xa5e1, 0xa5e2, 0xa5e4, 0xa5e6, 0xa5e8, 0xa5e9, 
	0xa5ea, 0xa5eb, 0xa5ec, 0xa5ed, 0xa5ef, 0xa5f3, 0xa1ab, 0xa1ac, 
};


/****************************************************/
/*
函数名:		strstoe2
功能：		______
参数：		char *dst：______, const char *src：______
返回值:		char ：______
*/
/**************************************************/
char *
strstoe2(char *buf, const char *src)
{
	const unsigned char *s = (unsigned char *)src;
	char *d = buf;
	while (*s != '\0') {
		int wc;
		if (is1sjis(*s)) {
			wc = GETWCH(s);
			wc = stoe(wc);
			if (s[1] != '\0')
				s++;
		}
		else if (isskana(*s)) {
			wc = kana_zen[*s - 0xa0];
		}
		else {
			wc = *s;
		}
		if ((wc & ~0xff) != 0) {
			*d++ = wc >> 8;
		}
		*d++ = wc;
		s++;
	}/* while */
	*d = '\0';
	return buf;
}/* strstoe2 */

/****************************************************/
/*
测试代码
*/
/**************************************************/
#ifdef TEST

char *myname;
char usage_msg[] =
	"euc/sjis converter V0.8086\n"
	"usage: %s [-es] [file ...]\n"
	"\t-e\tsjis->euc\n"
	"\t-j\tsjis->jis\n"
	"\t-s\teuc->sjis\n"
	;

int verbose;

int (*filter)(struct MB *mb, int ch, int (*put)(int)) = euc_to_sjis;

int
fnain(FILE *fp)
{
	struct MB mb;
	int err = 0, ch;
	filter(&mb, 0, putchar);
	while ((ch = fgetc(fp)) != EOF) {
		filter(&mb, ch, putchar);
	}/* while */
	filter(&mb, EOF, putchar);
	return err;
}/* fnain */

int
nain(const char *name)
{
	int err = -1;
	FILE *fp = fopen(name, "r");
	if (fp == NULL) {
		fprintf(stderr, "%s: %s: cannot open\n", myname, name);
	}
	else {
		err = fnain(fp);
		fclose(fp);
	}
	return err;
}/* nain */

#ifdef __BORLANDC__
extern int optind;
extern char *optarg;
extern int getopt(int, char **, const char *);
#endif

int
main(int argc, char *argv[])
{
	int ex, ch, show_usage = 0;
	myname = argv[0];
	while ((ch = getopt(argc, argv, "ejsvV")) != EOF) {
		switch (ch) {
		default:
		case 'V':
			show_usage++;
			break;
		case 'v':
			verbose = 1;
			break;
		case 'e':
			filter = sjis_to_euc;
			break;
		case 'j':
			filter = sjis_to_jis;
			break;
		case 's':
			filter = euc_to_sjis;
			break;
		}/* switch */
	}/* while */
	ex = 1;
	if (show_usage) {
		fprintf(stderr, usage_msg, myname);
	}
	else {
		ex = 0;
		if (argc - optind == 0) {
			if (fnain(stdin) < 0)
				ex = 1;
		}
		else {
			int i;
			for (i = optind; i < argc; i++) {
				if (nain(argv[i]) < 0)
					ex = 1;
			}/* for */
		}
	}
	return ex;
}/* main */

#endif
