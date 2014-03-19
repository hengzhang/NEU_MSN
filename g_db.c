/************************************************************/
/*
	1、定义与数据库相关的函数操作
		a.______——db_new
		b.______——db_free
		c.______——db_clear
		d.______——db_delete
		e.______——dbd_append
		f.______——dbd_free
		g.______——db_foreach
		h.______——db_count
		i.______——db_bsort

/*作者：______
/*日期：______
/**********************************************************/


#include <stdio.h>
#include <stdlib.h>
#include "g_db.h"


/****************************************************/
/*
函数名:		db_new
功能：		创建一个______
参数：		int (*comp)(const void *, const void *):指向函数comp的指针，函数comp的参数______
返回值:		struct DB_T *p:指向结构体DB_T的指针
*/
/**************************************************/
struct DB_T *db_new(int (*comp)(const void *, const void *))
{
	struct DB_T	*db;

	db = malloc(sizeof(struct DB_T));
	db->num = 0;
	db->top = 0;
	db->end = 0;
	db->comp = comp;
	return db;
}



/****************************************************/
/*
函数名:		db_free
功能：		将______进行释放
参数：		struct DB_T *db：一个指向结构体DB_T的指针
返回值:		void
*/
/**************************************************/
void db_free(struct DB_T *db)
{
	db_clear(db);
	free(db);
}


/****************************************************/
/*
函数名:		db_clear
功能：		将______进行清空
参数：		struct DB_T *db：一个指向结构体DB_T的指针
返回值:		void
*/
/**************************************************/
void db_clear(struct DB_T *db)
{
	struct DB_DAT_T	*dat;
	struct DB_DAT_T	*next;

	for (dat = db->top; dat != 0; dat = next) {
		next = dat->next;
		dbd_free(db, dat);
	}
}


/****************************************************/
/*
函数名:		db_delete
功能：		删除______
参数：		struct DB_T *db：一个指向结构体DB_T的指针
返回值:		void
*/
/**************************************************/
void db_delete(struct DB_T *db)
{
	struct DB_DAT_T	*dat;
	struct DB_DAT_T	*next;

	//遍历整个链表，删除所有节点
	for (dat = db->top; dat != 0; dat = next) {
		if (dat->data) free(dat->data);
		next = dat->next;
		dbd_free(db, dat);
	}
}


/****************************************************/
/*
函数名:		dbd_append
功能：		______
参数：		struct DB_T *db：一个指向结构体DB_T的指针, void *data：______
返回值:		struct DB_DAT_T *p:指向结构体DB_DAT_T的指针
*/
/**************************************************/
struct DB_DAT_T *dbd_append(struct DB_T *db, void *data)
{
	struct DB_DAT_T *dat;

	//分配内存空间
	dat = malloc(sizeof(struct DB_DAT_T));
	dat->next = 0;
	dat->prev = db->end;
	
	//如果链表不为空，则将节点加入到表尾
	if (db->end != 0)	db->end->next = dat;
		
	//如果链表为空，则将节点做为头结点
	if (db->top == 0)	db->top = dat;
	db->end = dat;
	dat->data = data;
	db->num++;
	return dat;
}


/****************************************************/
/*
函数名:		dbd_free
功能：		将______进行释放
参数：		struct DB_T *db：一个指向结构体DB_T的指针, struct DB_DAT_T *dat:一个指向结构体DB_DAT_T的指针
返回值:		void
*/
/**************************************************/
void dbd_free(struct DB_T *db, struct DB_DAT_T *dat)
{
	//如果dat为头结点，则头指针重新定位到下一节点
	//否则直接修改链接指针
	if (dat == db->top)	db->top = dat->next;
	else				dat->prev->next = dat->next;
		
	//如果dat为尾节点，则将尾指针指向前一节点
	//否则直接修改链接指针
	if (dat == db->end)	db->end = dat->prev;
	else				dat->next->prev = dat->prev;
	db->num--;
	free(dat);
}


/****************************************************/
/*
函数名:		db_foreach
功能：		______
参数：		struct DB_T *db：一个指向结构体DB_T的指针, int (*func)(struct DB_DAT_T *, void *)：______, void *data:______
返回值:		int : ______
*/
/**************************************************/
int db_foreach(struct DB_T *db, int (*func)(struct DB_DAT_T	*, void *), void *data)
{
	struct DB_DAT_T	*dat;
	int		ret;

	//遍历链表，直到func不为0或者遍历到链表结尾
	for (dat = db->top; dat != NULL; dat = dat->next) {
		if ((ret = func(dat, data)) != 0)	return ret;
	}
	return 0;
}


/****************************************************/
/*
函数名:		db_count
功能：		______
参数：		struct DB_T *db：一个指向结构体DB_T的指针, int (*func)(struct DB_DAT_T *, void *)：______, void *data:______
返回值:		int : ______
*/
/**************************************************/
int db_count(struct DB_T *db, int (*func)(struct DB_DAT_T	*, void *), void *data)
{
	struct DB_DAT_T	*dat;
	int		count = 0;

	//遍历链表，计算func不为0的节点数
	for (dat = db->top; dat != 0; dat = dat->next) {
		if (func(dat, data) != 0)	count++;
	}
	return count;
}


/****************************************************/
/*
函数名:		db_bsort
功能：		______
参数：		struct DB_T *db：一个指向结构体DB_T的指针, int (*func)(struct DB_DAT_T *, void *)：______, void *data:______
返回值:		int : ______
*/
/**************************************************/
int db_bsort(struct DB_T *db, int (*comp)(struct DB_DAT_T *, struct DB_DAT_T *))
{
	struct DB_DAT_T	*dat;
	struct DB_DAT_T	*dat_top;
	struct DB_DAT_T	*dat_next;
	struct DB_DAT_T	*dat_i;
	int		c;
	struct DB_DAT_T	*t_dat;
	dat_top = db->top;
	
	//遍历链表
	for (dat = dat_top; dat != 0; dat = dat_next) {
		if (dat->next == 0)	break;
		dat_next = dat->next;

		//从尾部开始遍历
		for (dat_i = db->end; dat_i != dat_top; dat_i = dat_i->prev) {
			if (dat == dat_i)	break;
			if (dat_i == 0) {
				g_print("!!!!!!!!!  NULL  !!!!!!!!!!!!!\n");
				break;
			}
			//比较dat,dat_i，并进行排序
			c = comp(dat, dat_i);
			if (c > 0) {
				//g_print("swap\n");
				if (dat_top == dat) {
					dat_top = dat_i;
				}
				if (db->top == dat)		db->top = dat_i;
				if (db->end == dat_i)	db->end = dat;

				if (dat->prev != 0)		dat->prev->next = dat_i;
				t_dat = dat->next;
				dat->next = dat_i->next;
				if (t_dat != dat_i)	dat_i->next = t_dat;
				else				dat_i->next = dat;
				if (dat_i->prev != dat)	dat_i->prev->next = dat;

				t_dat = dat_i->prev;
				dat_i->prev = dat->prev;
				if (dat->next != 0)		dat->next->prev = dat;
				dat->prev = t_dat;
				if (dat_i->next != 0)	dat_i->next->prev = dat_i;

				dat_next = dat_top;
				break;
			}
		}
		if (dat_i == dat) {
			dat_top = dat_top->next;
		}
	}

	return 1;
}

