/* Λιστα */

#include "list.h"
#include <stdlib.h>


alist *new_list(int id1)
{
 alist *li;
 static int id=0;
 li = (alist *)malloc(sizeof(struct alist));
 li->head = NULL;
 li->last = NULL;
 li->current = NULL;
 if(id1!=0) li->id = id1;
 else { 
 	id++;
	li->id = id;
 };
 li->f=0;
// l->size = el_size;
// printf("new list created at address: %ld\n",(long)li);
 return (li);
}


void add_element_to_list(void *p, alist *pl)
{
 _el *new_pel;
// printf("add_element_to_list: %d\n",p->id);
 /* create the new element */
 new_pel = (_el *)malloc(sizeof(struct _el));
 new_pel->data = (void *)p;
 if(pl->head==NULL) {
 	pl->head=new_pel;
	pl->last=new_pel;
	new_pel->prev=NULL;
 } else {
	new_pel->prev=pl->last;
	pl->last->next=new_pel;
 	pl->last=new_pel;
 };
 new_pel->next=NULL;
//  printf("element added:\n");
}

/* close list (add the first element at the end */
void close_list(alist *list)
{
 void *d;
 d = list->head->data;
 add_element_to_list(d,list);
}

// insert an element after
int insert_beetween1(alist *list,void *d,void *d1,void *d2)
{
 _el *e1,*e2,*ei;
 e1 = find_el(d1,list);
 e2 = e1->next;
 if(e2->data!=d2) { 
 	printf("we have a mismatch sequence !!!!!!!!!!!!!\n");
 	return(0);	
 };
 ei = (_el *)malloc(sizeof(struct _el));
 e1->next = ei;
 e2->prev = ei;
 ei->prev=e1;
 ei->next=e2;
 ei->data = d;
 return(1);
}

int insert_beetween(alist *list,void *d,void *d1,void *d2)
{
 _el *e1,*e2,*ei;
 e1 = find_el(d1,list);
 if(!e1) return(0);
 e2 = e1->next;
 if(e2->data!=d2) {
 
 	e1=find_el(d2,list);
	if(!e1) return(0);
	e2=e1->next;
	if(e2->data!=d1) {
 	printf("we have a mismatch sequence !!!!!!!!!!!!!\n");
 	return(0);	
	};
 };
 ei = (_el *)malloc(sizeof(struct _el));
 e1->next = ei;
 e2->prev = ei;
 ei->prev=e1;
 ei->next=e2;
 ei->data = d;
 return(1);
}


void lbegin(alist *l)
{
 l->current = l->head;
}

void *lget(alist *l)
{
 void *d;
 if(l->current == NULL) return NULL;
 d=l->current->data;
 l->current=l->current->next;
 return(d);
} 

/* return the n-th element of a list */
void *lget_num(alist *pl,int n)
{
 int i;
 _el *pel;
 void *p;
 p=NULL;
// printf("get data of %d element from list: %ld\n",n,(long)pl);
 for(i=0,pel=pl->head;i<n+1;i++,pel=pel->next) {
	if(pel==NULL) break;
	if(i==n) p=pel->data;
// 	if(pel == pl->last) break;
 };
 if(pel!=NULL) pl->current=pel;
 else pl->current=NULL;
// printf("i=%d ind=%d\n",i,ind);
 return (p);
}

/* find the element of a object in a list */
_el *find_el(void *obj, alist *pl)
{
 _el *pi;
// printf("find_pel:\n");
 for(pi=pl->head;pi!=NULL;pi=pi->next) {
 	if(pi->data == obj) return pi;
 };
 return NULL;
}

/* remove an object from a list */
/* just deletes the point element from that list not the point! */
/* because the point can be in other lists to of an object */
/* returns 1 if deleted,0 if not found */
int  remove_from_list(void *p, alist *list)
{
 _el *pi,*prev;
// printf("remove_point: \n");
 pi = find_el(p,list);
 if(pi) {
 	if(list->head == pi) {	// this is the head to remove
		if(list->head == list->last) { // this is the only element!
			list->head=NULL;
			list->last=NULL;
		} else {
		list->head = list->head->next;
		list->head->prev=NULL;
		if(list->last->data == p) { // this is a closed list
			list->last->data = list->head->data;	// close this properly too
		};
//		if(pi == list->last) list->last=NULL;
		};
	} else 
	if(list->last == pi) {	// this hapens only in not closed list!
		prev = pi->prev;
		prev->next = NULL;
		list->last=pi->prev;
	} else {
		pi->prev->next = pi->next;
		pi->next->prev = pi->prev;
	};
	free(pi);
	return 1;
 };
 return 0;
}

void free_list(alist *list)
{
 _el *pel;
 _el *peln;
 for(pel=list->head;pel!=NULL;pel=peln) {
 	peln=pel->next;
	free(pel);
 };
 free(list);
}

/* show debuging info of a list */
void show_alist(alist *pl)
{
 _el *pel;
 for(pel=pl->head;pel!=NULL;pel=pel->next) {
  printf(": %ld p=%ld n=%ld\n",(long)pel,(long)pel->prev,(long)pel->next);
 };
 if(pl->last!=NULL) printf("last: %ld p=%ld n=%ld\n",(long)pl->last,(long)pl->last->prev,(long)pl->last->next);
}

