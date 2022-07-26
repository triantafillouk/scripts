#ifndef __3DELEMENTS_KT__
#define	__3DELEMENTS_KT__

#include "list.h"

/* Point structure (position, color) */
typedef struct point
{
  float x;
  float y;
  float z;
  float r;
  float g;
  float b;
  int id;	// point id for debugging 
  int it;	// iteration number
  alist *connected;	// an akmi list the point belongs to
} point;


typedef struct akmi
{
 point *p[2];
 int id;	// id of akmi
 int it;	// iteration
 int f;		// flag for whatever use (flags that it is proccessed for next iteration)
 // we should have here a plane list
 point *midpoint;
 int num_planes; 	// number of planes connected at this akmi
 alist *connected;	// connected planes at this akmi
} akmi;

/* plane structure  */
/* plane is a list of points */
typedef struct plane
{
 int id;
 int it;	// iteration
 struct alist *pl;	// point list
} plane;


/* we need a plane list to draw an object */
/* we also need a point list to convert the object */
typedef struct object
{
 int id;	// object id
 alist *planes;	// plane list
 alist *points;	// point list
 alist *akmes;	// akmes list
} object;


/* Macros */
#define add_to_list(element , list)	add_element_to_list((void *)element,list)
#define add_point_to_object(p,object) add_to_list((void *)p, object->points)
#define add_point_to_list(p, list)	add_to_list((void *)p,list)
#define add_plane_to_list(p, list)	add_to_list((void *)p,list)
#define add_akmi_to_list(p, list)	add_to_list((void *)p,list)

#define get_first_point(list) (point *)lget_num(list,0)
#define get_next_point(list) (point *)lget(list)

#define get_first_plane(list) (plane *)lget_num(list,0)
#define get_next_plane(list) (plane *)lget(list)

#define get_first_akmi(list) (akmi *)lget_num(list,0)
#define get_next_akmi(list) (akmi *)lget(list)

#define insert_beetween_points(list,p,p1,p2) insert_beetween(list,(void *)p,(void *)p1,(void *)p2)

#endif
