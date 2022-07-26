#ifndef __LIST_KT__
#define	__LIST_KT__
#include <stdio.h>
/* list structure definitions */

typedef struct _el
{
 void *data;
 struct _el *next;
 struct _el *prev;
} _el;

typedef struct alist
{
 int id;	// list id
 struct _el *head;
 struct _el *last;
 struct _el *current;
 int size;	// size of the data to store
 int f;		// generic flag
} alist;

/* function declarations */
alist *new_list(int id);
void add_element_to_list(void *p, alist *pl);
void lbegin(alist *l);
void *lget(alist *l);
void *lget_num(alist *pl,int n);
int  remove_from_list(void *p, alist *pl);
_el *find_el(void *p, alist *pl);
int insert_beetween(alist *l,void *d,void *d1,void *d2);
void free_list(alist *list);
#endif
