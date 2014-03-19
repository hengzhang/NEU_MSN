/********************************************/
/*名称：g_db.h
/*描述：______
/*作者：______
/*日期：______
/********************************************/


#ifndef DB_H__
#define	DB_H__
/******************************************************/
/*	定义与______相关的结构体DB_T：
	num：______
	top：______
	end:______
	int (*comp)(const void *, const void *)：______
/******************************************************/
typedef struct DB_T {
	unsigned int	num;
	struct DB_DAT_T	*top;
	struct DB_DAT_T	*end;
	int (*comp)(const void *, const void *); /* default€ÎÈæ³ÓŽØ¿ô */
} DB;


/******************************************************/
/*	定义与______相关的结构体DB_DAT_T：
	data：______
	next：______
	prev:______
/******************************************************/
typedef struct DB_DAT_T {
	void			*data;
	struct DB_DAT_T		*next;
	struct DB_DAT_T		*prev;
} DB_DAT;

struct DB_T *db_new(int (*comp)(const void *, const void *));
void db_free(struct DB_T *db);
void db_clear(struct DB_T *db);
void db_delete(struct DB_T *db);

struct DB_DAT_T *dbd_append(struct DB_T *db, void *data);

void dbd_free(struct DB_T *db, struct DB_DAT_T *dat);
int db_foreach(struct DB_T *db, int (*func)(struct DB_DAT_T *, void *), void *data);
int db_count(struct DB_T *db, int (*func)(struct DB_DAT_T *, void *), void *data);
int db_bsort(struct DB_T *db, int (*comp)(struct DB_DAT_T *, struct DB_DAT_T *));


#endif /* DB_H__ */
