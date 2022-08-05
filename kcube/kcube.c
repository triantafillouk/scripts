/********************************************************/
/*                     kcube.c                           */
/********************************************************/

#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include "list.h"
#include "3delements.h"


/* function declarations */
void iterate(object *ob);
void idle();

int iteration;
int p_iteration; // previous iteration

akmi *new_akmi(point *p1,point *p2,int it)
{
 static int id=0;
 akmi *a;
 a = (akmi *)malloc(sizeof(struct akmi));
 a->p[0]=p1;
 a->p[1]=p2;
 a->id = id++;
 a->f = 0;
 a->connected = new_list(0);	// list with planes that this akmi is connected
 a->num_planes = 0;
 a->midpoint = NULL;
 a->it = it;
 return(a);
}

/* create a new plane */
plane *new_plane(int iteration)
{
 plane *pla;
 static int id=0;
// printf("new_plane: id=%d\n",id);
 pla = (plane *)malloc(sizeof(struct plane));
 pla->id = id++;
 pla->it = iteration;
 pla->pl=new_list(0);
 return (pla);
}

/* create an object */
object *new_object()
{
 object *ob;
 static int id=0;
// printf("new_object: id=%d\n",id);
 ob = (object *)malloc(sizeof(struct object));
 ob->id=id++;
 ob->planes = new_list(0);
 ob->points = new_list(0);
 ob->akmes = new_list(0);
// printf("object plist = %d\n",ob->pl->id);
 return(ob);
}

/* create vertexes with the points of the plane */
void draw_plane(plane *pla)
{
 point *p;
// printf("draw_plane: id=%d : ",pla->id);
 glBegin(GL_POLYGON);

 for(p=get_first_point(pla->pl);p!=NULL;p=get_next_point(pla->pl))
 {
//	printf(" %d ",p->id);
	glColor3f(p->r,p->g,p->b);
	glVertex3f(p->x,p->y,p->z);
 };
// printf("\n");
 glEnd();
}

/* draw all planes of the object */
void draw_object(object *ob)
{
 plane *pla;
// printf("draw_object:\n");
 int i=0;
// iterate for all planes of the object
 for(pla=get_first_plane(ob->planes);pla!=NULL;pla=get_next_plane(ob->planes))
 {
 	draw_plane(pla);
 };
}

void draw_axis(double len)
{
  glColor3f(1.0,0.0,0.0);
  glBegin(GL_LINES);
  	glVertex3f(0,0,0);glVertex3f(len,0,0);
  glEnd();
  glColor3f(0.0,1.0,0.0);
  glBegin(GL_LINES);
  	glVertex3f(0,0,0);glVertex3f(0,len,0);
  glEnd();
  glColor3f(0.0,0.0,1.0);
  glBegin(GL_LINES);
  	glVertex3f(0,0,0);glVertex3f(0,0,len);
  glEnd();
}

/* if 2 points are connected returs theirs akmi */
akmi *akmi_of_points(point *p1,point *p2)
{
 /* find if point p2 is in the connected list of p1 */
 /* find if in the connected (akmi) list of point p1 exist point p2 */
 _el *ael;
 akmi *a;
 alist *al = p1->connected;
// printf("check if points are connected\n");
 for(ael = al->head; ael!=NULL; ael=ael->next) {
 	a = (akmi *)ael->data;
	if(a->p[0] == p2) return(a);
	if(a->p[1] == p2) return(a);
 };
 return 0;
}

/* true if 2 points are connected */
int points_connected(point *p1,point *p2)
{
 if(akmi_of_points(p1,p2)!=NULL) return 1;
 else return 0;
}

/* find the plane defined by to akmes */
plane *plane_of_akmes(akmi *a1,akmi *a2)
{
 plane *pla1,*pla2;
 for(pla1=get_first_plane(a1->connected);pla1!=NULL;pla1=get_next_plane(a1->connected)) {
	for(pla2=get_first_plane(a2->connected);pla2!=NULL;pla2=get_next_plane(a2->connected)) {
 	if(pla1==pla2) return(pla1);
 }};
 return(NULL);
}

/* create an akmi */
akmi *connect_points(point *p1,point *p2,object *ob)
{
 akmi *a;
// printf("connect_points: %d - %d\n",p1->id,p2->id);
 if((a=akmi_of_points(p1,p2))!=NULL) return(a);// already connected!
 a = new_akmi(p1,p2,iteration);
 add_akmi_to_list(a,p2->connected);
 add_akmi_to_list(a,p1->connected);
 add_akmi_to_list(a,ob->akmes);
// printf("points connected!\n");
 return (a);
}

void show_object_connections(object *ob)
{
 akmi *a;
 point *p;
 int i;
 for(p=get_first_point(ob->points);p!=NULL;p=get_next_point(ob->points))
 {
 	printf("p[%2d]:  ",p->id);
	for(a=get_first_akmi(p->connected);a!=NULL;a=get_next_akmi(p->connected))
	{
		for(i=0;i<2;i++) {
			if(p == a->p[i]) continue;
			printf(" %d",a->p[i]->id);
		};	
	};printf("   akmes ");
	for(a=get_first_akmi(p->connected);a!=NULL;a=get_next_akmi(p->connected))
	{
		printf(" %d ",a->id);
	};
	printf("\n");
 };
}

void show_akmes(object *ob)
{
 akmi *a;
 int i;
 plane *pla;
 for(a=get_first_akmi(ob->akmes);a!=NULL;a=get_next_akmi(ob->akmes)) {
 	printf("akmi[%2d]: points ",a->id);
	for(i=0;i<2;i++) {
		printf(" %d",a->p[i]->id);
	};
	printf(" planes ");
	for(pla=get_first_plane(a->connected);pla!=NULL;pla=get_next_plane(a->connected)) {
		printf(" %d",pla->id);
	};
	printf("\n");
 };
}

void show_planes(object *ob)
{
 point *p1;
 plane *pla;
 printf("Object %d, iteration %d\n",ob->id,iteration);
 for(pla=get_first_plane(ob->planes);pla!=NULL;pla=get_next_plane(ob->planes)) {
	printf("plane %d points: ",pla->id);
	for(p1=get_first_point(pla->pl);p1!=NULL;p1=get_next_point(pla->pl)) {
		printf(" %d ",p1->id);
	};
	printf("\n");
 };
}

/* add a point at the end of the plane list */
/* returns an akmi in case of a connection */
akmi * add_point_to_plane(point *p, plane *pla,object *ob)
{
 point *pp;	// previous point in the list
 akmi *a;
 a=NULL;
// printf("add_point_to_plane: point %d to plane %d\n",p->id,pla->id);
// show_plist(pla->pl);
 /* if the last point is not NULL, connect points */
 add_point_to_list(p,pla->pl);
 if(pla->pl->last->prev!=NULL) {
	pp = (point *)pla->pl->last->prev->data;
	a=connect_points(pp,p,ob);
	if(a!=NULL) 
	{
//		printf("add plane to akmi connected list\n");
		add_plane_to_list(pla,a->connected);
		a->num_planes++;
	};
 };
 return(a);
// printf("point added to plane!\n");
}

/* create a new point */
point *new_point(float x,float y,float z, float r,float g, float b,int it)
{
 static int id=0;
 point *p;
// printf("new_point: id=%d it=%d\n",id,it);
 p = (point *)malloc(sizeof(struct point));
 p->x=x;
 p->y=y;
 p->z=z;
 p->r=r; p->g=g; p->b=b;
 p->id=id++;
 p->it=it;
 p->connected = new_list(0);
 return (p);
}

int  remove_point_from_plane(point *p, plane *pla)
{
 return(remove_from_list((void *)p,pla->pl));
}

int remove_plane_from_akmi(plane *pla,akmi *a)
{
   return 1;
}

int remove_akmi_from_point(akmi *a,point *p)
{
	return 1;
}

/* check if two akmes have a common plane */
int in_same_plane(akmi *a1,akmi *a2)
{
 int stat=0;
 plane *p1,*p2;
 for(p1=get_first_plane(a1->connected);p1!=NULL;p1=get_next_plane(a1->connected)) {
 	for(p2=get_first_plane(a2->connected);p2!=NULL;p2=get_next_plane(a2->connected)) {
 		if(p1==p2) return(1);
 	};
 };
 return(0);
}

/* return true in case that akmi is on plane */
int akmi_on_plane(akmi *a1,plane *pla)
{
 plane *p1;
 for(p1=get_first_plane(a1->connected);p1!=NULL;p1=get_next_plane(a1->connected)) {
 	if(p1==pla) return(1);
 };
 return(0);
}

/* reorder an akmi list so that consequitives akmes have a common plane */ 
/* note that the list is not closed! */
int reorder_akmi_list(point *p)
{
  plane *pla,*p1;
  akmi *a,*aprev,*a2;
  _el *ael;	// an akmi element
  int stat;
  alist *new;
  int not_all_akmes_added;
  a=get_first_akmi(p->connected);
  new=new_list(0);
  add_akmi_to_list(a,new);
//  printf("add akmi %d to list\n",a->id);
  a->f=1;
  pla=(plane *)a->connected->head->data;
  not_all_akmes_added=1;
  while(not_all_akmes_added) {
  not_all_akmes_added=0;
  for(a=get_first_akmi(p->connected);a!=NULL;a=get_next_akmi(p->connected)) {
  	if(a->f) continue;	// we already added this akmi!
//	printf("check akmi %d \n",a->id);getchar();
	if(akmi_on_plane(a,pla)) {
		add_akmi_to_list(a,new);
//		printf("add akmi %d to list\n",a->id);
		a->f=1;
		// set tha plane to be another plane in a->connected
		for(p1=get_first_plane(a->connected);p1!=NULL;p1=get_next_plane(a->connected)) {
			if(p1!=pla) {pla=p1;break;};
		};
		if(p1==NULL) {
			printf("No need for reoder, %d is the only plane in akmi %d\n",pla->id,a->id);
			return (0);
		};
		continue;
	};
	not_all_akmes_added=1;
  };
  };
  
  // init_akmes(new);	// set flag f to zero for all akmes in the list
  for(a=get_first_akmi(new);a!=NULL;a=get_next_akmi(new)) {
  	a->f=0;
  };
  // remove/free list old list p->connected
  free_list(p->connected);
  p->connected=new;
  return(1);
}

/* distance beetween two points */
double fdist(point *p0,point *p1)
{
 double d;
 d = (p1->x-p0->x)*(p1->x-p0->x)+(p1->y-p0->y)*(p1->y-p0->y)+(p1->z-p0->z)*(p1->z-p0->z);
 d = sqrt(d);
 return d;
}

/* returns a new point in the middle of 2 points */
point * new_mid_point(point *p0,point *p1, int it)
{
 point *p2;
// double a,d;
 double x,y,z,r,g,b;
// d = fdist(p0,p1);
 x = (p1->x + p0->x)/2;
 y = (p1->y + p0->y)/2;
 z = (p1->z + p0->z)/2;
 r = (p1->r + p0->r)/2;
 g = (p1->g + p0->g)/2;
 b = (p1->b + p0->b)/2;

 p2 = new_point(x,y,z,r,g,b,it);
 return p2;
}

/* returns the barycentric point of a plane */
point * new_barcentr(plane *pla,int it) 
{
 point *np;
 point *p;
 int n=0;
 double x=0,y=0,z=0,r=0,g=0,b=0; 
// printf("barycentric point of plane %d\n",pla->id);
 for(p=get_first_point(pla->pl);p!=NULL;p=get_next_point(pla->pl)) {
 	if(p==(point *)pla->pl->last->data && n>0) break;
	x += p->x;
	y += p->y;
	z += p->z;
//	 printf("x=%f y=%f z=%f\n",x,y,z);
	r += p->r; g += p->g; b += p->b;
	n++;
 };
 x /= n; y /= n; z /= n;
 r /= n; g /= n; b /= n;
 np = new_point(x,y,z,r,g,b,it);
// printf("x=%f y=%f z=%f\n",x,y,z);
 return(np);
}

double dist_point_center_plane(point *p1,plane *pla)
{
 double d;
 point *p0;
 p0 = new_barcentr(pla,-1);
 d = fdist(p0,p1);
// printf("distance of plane %d from 00 is %f\n",pla->id,d);
 return(d);
}

/* returns a new point at distance dist from point p0 in the line p0-p1 */
point * new_point_fd(point *p0,point *p1, double dist,int it)
{
 point *p2;
 double a,d;
 d = fdist(p0,p1);
 p2 = (point *)malloc(sizeof(struct point));
 if(d > 0.000001) {
	a = dist/d;
 	p2->x = (p1->x - p0->x)/a + p0->x;
 	p2->y = (p1->y - p0->y)/a + p0->y;
 	p2->z = (p1->z - p0->z)/a + p0->z;
 	p2->r = (p1->r - p0->r)/a + p0->r;
 	p2->g = (p1->g - p0->g)/a + p0->g;
 	p2->b = (p1->b - p0->b)/a + p0->b;
 } else {
   	p2->x = p0->x;p2->y = p0->y;p2->z = p0->z;
	p2->r = p0->r;p2->g = p0->g;p2->b = p0->b;
 };
 p2->connected = new_list(0);
 return p2;
}

void insert_point_in_plane(point *m1,plane *pla,akmi *a)
{
 point *p1,*p2;
// printf("insert midpoint %d on plane %d from akmi %d (%d,%d)\n",m1->id,pla->id,a->id,a->p[0]->id,a->p[1]->id);
 if(!insert_beetween_points(pla->pl,m1,a->p[0],a->p[1])) {
 	printf("cannot insert point beetween %d and %d\n",a->p[0]->id,a->p[1]->id);
 };
// remove_akmi_from_plane(pla,a);
}

point *p00;	// zero point 
double max_pdist=0;		// maximum point distance
double min_pdist=10.0;	// minimum point distance
double max_adist=0;		// center point distance
double min_adist=10.0;
int  show_sphere=1;
int total_points=0;
int total_akmes=0;
int total_planes=0;

point p_cube[8]={
    {-0.5,-0.5, 0.5 ,0.0,0.0,1.0,0},
    {-0.5, 0.5, 0.5 ,0.0,1.0,1.0,0},
    { 0.5, 0.5, 0.5 ,1.0,1.0,1.0,0},
    { 0.5,-0.5, 0.5 ,1.0,0.0,1.0,0},
    {-0.5,-0.5,-0.5 ,0.2,0.2,0.2,0},
    {-0.5, 0.5,-0.5 ,0.0,1.0,0.0,0},
    { 0.5, 0.5,-0.5 ,1.0,1.0,0.0,0},
    { 0.5,-0.5,-0.5 ,1.0,0.0,0.0,0}
};

/* these define the points of the planes of the cube */
int f[6][5]={
  {0,1,2,3,0},
//  {0,1,3,7,0},
  {3,2,6,7,3},
  {4,5,6,7,4},
  {0,1,5,4,0},
  {1,5,6,2,1},
  {0,4,7,3,0}};



object * new_cube()
{
 object *cube1;
 point *p;
 akmi *a;
 int i;
 double d;
 cube1 = new_object();
 // define the points of the cube, add them to the point list
 for(i=0;i<8;i++) {
 	p = new_point(p_cube[i].x,p_cube[i].y,p_cube[i].z,p_cube[i].r,p_cube[i].g,p_cube[i].b,iteration);
	d = fdist(p,p00);
	if(d>max_pdist) max_pdist=d;
	if(d<min_pdist) min_pdist=d;
	add_point_to_object(p,cube1);
 };
 // define the planes
 for(i=0;i<6;i++) {
	// define the plane
	plane *pla;
	int j;
	pla = new_plane(0);

	for(j=0;j<5;j++) {
		// add the point to the plane
		a=add_point_to_plane( lget_num(cube1->points,f[i][j]),pla,cube1);
	};
	
		// add the planes to the cube
//		printf("add plane %d to cube\n",pla->id);
		add_plane_to_list(pla,cube1->planes);

		d = dist_point_center_plane(p00,pla);
//		printf("plane %d dist %f\n",pla->id,d);
		if(d>max_adist) max_adist=d;
		if(d<min_adist) min_adist=d;
		
 };
// printf("all planes added!\n");
// show_planes(cube1);
// show_akmes(cube1);
 printf("-------------------------------------------\n");
 return(cube1);
}


char pressed;
int anglex,angley,anglez,y,xold,yold;

/* Prototype des fonctions */
void display();
void keyb(unsigned char touche,int x,int y);
void reshape(int x,int y);
void idle();
void atimer(int i);
int atimerf=0;
int updatingf=0;

void mouse(int bouton,int etat,int x,int y);
void mousemotion(int x,int y);
void special(int k, int x, int y);

object *cube1;
object *cube0;
//int iteration;

int main(int argc,char **argv)
{
 char *window_title="Cube";
  anglex=20;// 20
  angley=10;// 10
  anglez=0;
  iteration=0;
  p00 = new_point(0,0,0,0,0,0,-1);
  cube0=new_cube();
  cube1=new_cube();
  total_points=8;
  total_akmes=12;
  total_planes=6;
  printf("max_pdist = %f, min_pdist=%f\n",max_pdist,min_pdist);
  printf("max_adist = %f, min_adist=%f\n",max_adist,min_adist);
  printf("----------------------------------\n");
//  show_object_connections(cube1);
  /* initialisation of glut and window */
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowPosition(400,100);
  glutInitWindowSize(400,400);
  glutCreateWindow(window_title);

  /* Initialisation d'OpenGL */
  glClearColor(0.0,0.0,0.0,0.0);
  glColor3f(1.0,1.0,1.0);
  glPointSize(2.0);
  glEnable(GL_DEPTH_TEST);

  /* enregistrement des fonctions de rappel */
  glutDisplayFunc(display);
  glutKeyboardFunc(keyb);
  glutReshapeFunc(reshape);
  glutMouseFunc(mouse);
  glutMotionFunc(mousemotion);
  glutSpecialFunc(special);
  glutTimerFunc(100,atimer,100);
//  glutIdleFunc(idle);

  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
 
  /* Entree dans la boucle principale glut */
  glutMainLoop();
  return 0;
}


void display()
{
  int i,j;
  /* clear with background color */
//  printf("start drawing\n");
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  glRotatef(-angley,1.0,0.0,0.0);
  glRotatef(-anglex,0.0,1.0,0.0);
  glRotatef(-anglez,0.0,0.0,1.0);
//  draw_object(axis_cube);
  draw_axis(2.0);
  draw_object(cube0);
  draw_object(cube1);
  if(show_sphere) {
  	glColor3f(0.5,0.5,0.5);
  	glutWireSphere(0.5,50,50);
  }
  glFlush();
  
  /* swap display buffers */
  glutSwapBuffers();
  updatingf=0;
//  printf("end drawing!\n");
}

void keyb(unsigned char key,int x,int y)
{
  switch (key)
    {
    case 'f': /* full mode display */
      glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
      glutPostRedisplay();
      break;
    case 'l': /* line mode display */
      glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
      glutPostRedisplay();
      break;
    case 's' : /* point mode display */
      glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);
      glutPostRedisplay();
      break;
	case 'a': // start/stop timer
		if(atimerf) atimerf=0;else atimerf=1;
		glutTimerFunc(100,atimer,100);
		break;
	case 'p': // show sphere
		if(show_sphere) show_sphere=0;else show_sphere=1;
	    glutPostRedisplay();
		break;
    case 'd':
      glEnable(GL_DEPTH_TEST);
      glutPostRedisplay();
      break;
    case 'D':
      glDisable(GL_DEPTH_TEST);
      glutPostRedisplay();
      break;
	case 'i':
		iterate(cube1);
		glutPostRedisplay();
		break;
    case 'q' : /* press 'q' to quit */
      exit(0);
    }
}

void idle()
{
#if	0
 static int i=0;
 if(atimerf) {
 	printf("idle %d\n",i++);
	atimerf=0;
 };
#endif
}

void atimer(int i)
{
 int static j=0;
 int static current=0;
 int static f=0;
 
 if(f) {
 	printf("loop %d still working on %d !\n",current,j);
 } else {
 if(atimerf) {
// printf("atimer %d\n",j++);
 anglex ++;
 glutTimerFunc(i,atimer,i);
 if(updatingf!=1) {
 	updatingf=1;
//	printf("post redisplay!\n");
	glutPostRedisplay();j=current;
 	
 }else {
	printf("loop %d still working on %d !\n",current,j);
 };;
 };
 };
 current++;
}

void reshape(int x,int y)
{
  if (x<y)
    glViewport(0,(y-x)/2,x,x);
  else 
    glViewport((x-y)/2,0,y,y);
}

void mouse(int button, int state,int x,int y)
{
  /* press left mouse button */
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) 
  {
    pressed = 1; /* mouse button is pressed */
    xold = x; /* old value of mouse position */
    yold=y;
  }
  /* leave left mouse button */
  if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) 
    pressed=0; /* mouse button is not pressed */
}

void mousemotion(int x,int y)
  {
    if (pressed) /* in case that the left mouse button is pressed */
    {
      /*
	 Change the rotation angles of the object using the difference of the actual
	 mouse position and the previous mouse position.
	 */
      anglex=anglex+(x-xold); 
      angley=angley+(y-yold);
      glutPostRedisplay(); /* refresh the display */
    }
    
    xold=x; /* save current mouse position as old values for next comparison */
    yold=y;
  }

/* special keyboard handling */
/* us it to rotate object */
void special(int k, int x, int y)
{
  switch (k) {
  case GLUT_KEY_UP:
    angley -= 5.0;
    break;
  case GLUT_KEY_DOWN:
    angley += 5.0;
    break;
  case GLUT_KEY_LEFT:
    anglex -= 5.0;
    break;
  case GLUT_KEY_RIGHT:
    anglex += 5.0;
    break;
  case GLUT_KEY_PAGE_UP:
    anglez += 5.0;
    break;
  case GLUT_KEY_PAGE_DOWN:
    anglez -= 5.0;
    break;
  default:
    return;
  }
  glutPostRedisplay();
}

/* add pla1 to connected list of a1, ap is the another akmi of the plane */
void add_side_plane_to_akmi(akmi *a1,akmi *ap,akmi *an)
{
 plane *pla1;
 if(a1==NULL || ap==NULL) return;
// printf("add side plane to akmi %d (ap=%d an=%d)\n",a1->id,ap->id,an->id);
 pla1 = plane_of_akmes(an,ap);
 if(pla1) add_plane_to_list(pla1,a1->connected);
 else { printf("cannot find a common plane of akmes %d,%d\n",a1->id,ap->id);};
// printf("added plane %d to akmi %d\n",pla1->id,a1->id);
}

void iterate(object *ob)
{
 plane *pla0,*pla;
 point *p0,*m1,*p1;
 akmi *a;
 akmi *a1,*a0; 
 akmi *ap; // previous akmi
 int new_points=0,new_planes=0,new_akmes=0;
 int deleted_points=0,deleted_planes=0,deleted_akmes=0;

 /* set iteration number */
 p_iteration=iteration;
 iteration++;
 max_pdist=0;
 min_pdist=10;
 
// show_akmes(ob);
 for(p0=get_first_point(ob->points);p0!=NULL;p0=get_next_point(ob->points)) {
	reorder_akmi_list(p0);
 };
// show_object_connections(ob);

// 1. For each point P0 we create a new plane PLA0.
 for(p0=get_first_point(ob->points);p0!=NULL;p0=get_next_point(ob->points)) {
	double d;
 	if(p0->it == iteration) break;
 	pla0 = new_plane(iteration);
	new_planes++;

//	The plane will be defined by the mid points of the points of acnes that connect at P0, so
//		For each acne that connects with the point P0
		for(a0=a=get_first_akmi(p0->connected),p1=NULL,ap=NULL;a!=NULL;ap=a,a=get_next_akmi(p0->connected)) {
//		- Creation of mid point M1 (if not already created!).
			if(a->midpoint == NULL) {
				m1 = new_mid_point(a->p[0],a->p[1],iteration);

				d = fdist(m1,p00);
				if(d>max_pdist) max_pdist=d;
				if(d<min_pdist) min_pdist=d;

				new_points++;
				a->midpoint = m1;
				add_point_to_object(m1,ob);
				a->f = 1;	// set the flag that we have proccessed the akmi 
//		- For each plane the acne belongs to:
				for(pla=get_first_plane(a->connected);pla!=NULL;pla=get_next_plane(a->connected)) {
//		- insert of point M1 at plane PLA beetween the 2 points that define the acne.
					insert_point_in_plane(m1,pla,a);
				};
			} else m1 = a->midpoint;
//		- add the point M1 to PLA0
			if(p1==NULL) p1=m1;	// store the first point of the plane
			a1=add_point_to_plane(m1,pla0,ob);
			if(a1) new_akmes++;
			add_side_plane_to_akmi(a1,ap,a);
//			printf("<%d>",m1->id);
		};
		// close the plane!
		a1=add_point_to_plane(p1,pla0,ob);
		new_akmes++;
		add_side_plane_to_akmi(a1,ap,a0);
//		printf("<%d>\n",p1->id);
		d = dist_point_center_plane(p00,pla0);
//		printf("plane %d dist %f\n",pla0->id,d);
		if(d>max_adist) max_adist=d;
		if(d<min_adist) min_adist=d;

		add_plane_to_list(pla0,ob->planes);
 };
// show_planes(ob);
//	2. Remove all the points of the previous iteration from all planes of the previous iteration.
	for(pla=get_first_plane(ob->planes);pla!=NULL;pla=get_next_plane(ob->planes)) {
		for(p1=get_first_point(pla->pl);p1!=NULL;p1=get_next_point(pla->pl)) {
			if(p1->it == p_iteration) remove_point_from_plane(p1,pla);
		};
	};
//	- Remove all acnes of previous iteration and create new ones.
//	- Remove all points of previous iteration from the point list.
	for(p1=get_first_point(ob->points);p1!=NULL;p1=get_next_point(ob->points)) {
		if(p1->it == p_iteration) {
//			if(iteration<4) printf("remove point %d %d\n",p1->id,p1->it);
			remove_from_list((void *)p1,ob->points);
			deleted_points++;
		}
	};
//	4. Remove all acnes of previous iteration
	for(a=get_first_akmi(ob->akmes);a!=NULL;a=get_next_akmi(ob->akmes)) {
		if(a->it == p_iteration) {
			remove_from_list((void *)a,ob->akmes);
			deleted_akmes++;
		};
	};
	printf("max_pdist = %f, min_pdist=%f\n",max_pdist,min_pdist);
	printf("max_adist = %f, min_adist=%f\n",max_adist,min_adist);
//  	printf("iteration %d: added point %d,added planes %d,added akmes %d,!\n",iteration,new_points,new_planes,new_akmes);
//  	printf("  deleted points %d,deleted planes %d,deleted akmes %d,!\n",deleted_points,deleted_planes,deleted_akmes);
	total_points += new_points - deleted_points;
	total_akmes  += new_akmes - deleted_akmes;
	total_planes += new_planes;
	printf("total %d points, %d akmes, %d planes\n",total_points,total_akmes,total_planes);	
	// show the planes
//	show_planes(ob);
//	show_object_connections(ob);
	printf("-------------------------------------------\n");
}
