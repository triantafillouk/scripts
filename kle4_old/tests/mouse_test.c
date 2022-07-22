/*
	Curses,gtk editor,notes,directory browser
	Copyright Kostas Triantafillou
	GNU LESSER GENERAL PUBLIC LICENSE version 2 
	(or at your option) any later version.

	curses mouse test utility
*/

//#include	"xe.h"
#include	<locale.h>
#define	_XOPEN_SOURCE_EXTENDED	1
#include	<locale.h>
#include	<curses.h>
#include	<stdlib.h>
#include	<panel.h>
#include	<term.h>
//#include	"color.h"
#include	<sys/signal.h>
#include	<termios.h>
#include	<sys/ioctl.h>
#include	<fcntl.h>
#include	<signal.h>
#include	<string.h>

#define CNTRL   0x0100          /* Control key flag    */
#define META    0x0200          /* Meta key flag       */
#define CTLX    0x0400          /* ^X flag             */
#define	SPEC	0x0800			/* special key (function keys)	*/
#define SPEC1	0x1000			/* first alternate keys		*/
#define SPEC2	0x2000			/* second alternate keys	*/
#define	X2UTF	0x4000			/* X11 to utf and insert  */
#define KMOUS	0x8000			/* kmous terminfo key  */

#define KMOUSE_NONE		0
#define KMOUSE_BUTTON1	1
#define KMOUSE_BUTTON2	2
#define KMOUSE_BUTTON3	3
#define KMOUSE_BUTTON4	4
#define KMOUSE_BUTTON5	5
#define KMOUSE_ENTER	6
#define KMOUSE_GOTO		7
#define KMOUSE_DBLCLICK	16
#define KMOUSE_RELEASE	64
#define KMOUSE_EXIT		128
#define KMOUSE_UPDATE	129

#define	CHR_ESC		0x1B		/* espape character */
#define	CHR_SPACE	0x20		/* space character	*/
#define	CHR_DEL		0x7F		/* delete character */
#define	CHR_CTLX	'X'-'@'		/* ^x character (24) */
#define CHR_LBRA	'['			/* left bracket  */
#define	CHR_RBRA	']'			/* right bracket  */

int mousex=0;	/* mouse position x */
int mousey=0;	/* mouse position y */
int mousepx=0;	/* mouse press position x */
int mousepy=0;	/* mouse press position y */
int mouserx=0;	/* mouse release position x */
int mousery=0;	/* mouse release position y */
int text_mouse_keys=0;	/* mouse keys recognised flagin curses */
//num text_mouse_time=0;	/* time max for double mouse click  */

char *text_mouse_str=NULL;	/* text mouse terminfo kmous string */
int mmouse=0;	/* 1 = ansi mouse, 0 = M mouse  */
int mouse_button=0;
int mouse_active=0;

void enable_key_mouse()
{
 mousemask(ALL_MOUSE_EVENTS|REPORT_MOUSE_POSITION,NULL);
 mousemask(ALL_MOUSE_EVENTS, NULL);

 printf("\033[?1003h");	/* makes terminal report mouse mouvement  */
 if(mmouse) printf("\033[?1006h");	/* makes terminal report mouse mouvement  */
 fflush(stdout);
 mouse_active=1;
}

void disable_key_mouse()
{
 mousemask(0,NULL);
 printf("\033[?1003l");	/* makes terminal stop reporting mouse mouvement  */
 if(mmouse) printf("\033[?1006l");	/* makes terminal stop reporting mouse mouvement  */
 fflush(stdout);
 mouse_active=0;
}

int getanum(int *end_key)
{
 int num=0;
 int c;
 while((c=getch()) >= '0' && c<='9'){
//	printf("%d\r\n",c);
 	num = num*10 + (c-'0');
 };
 *end_key = c;
// printf("num=%d c=%c\r\n",num,c);
 return num;
}

int main(int argc,char **argv)
{
 int c;
 int last_key=' ';

 initscr();
 clear();
 noecho();
// raw();
// nonl();
 cbreak();

 fprintf(stderr,"mouse_test\r\n");
	
	text_mouse_str = tigetstr("kmous");	/* ncurses mouse  */
	fprintf(stderr,"text_mouse = %d %c %c\n\r",text_mouse_str[0],text_mouse_str[1],text_mouse_str[2]);
	if(text_mouse_str !=NULL) {
		if(text_mouse_str[0]!=0) {
		text_mouse_keys=1;
		mouse_active=1;
		if(text_mouse_str[2]=='<') mmouse=1;

		enable_key_mouse();	
		fprintf(stderr,"found mouse! mmouse=%d (%c)\r\n",mmouse,text_mouse_str[2]);
		fflush(stderr);
		while((c=wgetch(stdscr))!='q') {
			if(c==CHR_ESC) {
//				fprintf(stderr,"ESC-");
				c=getch();
			};
			if(c==CHR_LBRA){		
//				fprintf(stderr,"[");
				c=getch();
			if(c==text_mouse_str[2]) {
			if(c=='M') {
				int c0=getch();
				mousex = getch()-33;
				mousey = getch()-33;
				fprintf(stderr,"basic mouse button=%d x=%d y=%d\r\n",c0,mousex,mousey);
			} else { /* extended mouse reporting!  */
				int mouse_button=0;
				mouse_button = getanum(&last_key);
				mousex = getanum(&last_key)-1;								
				mousey = getanum(&last_key)-1;
				fprintf(stderr, "extended mouse button=%d x=%d y=%d last=%c\r\n",mouse_button,mousex,mousey,last_key);
			}
			};

			} else {
				fprintf(stderr, "-- c=%d\n",c);
			};
		};

		disable_key_mouse();
		endwin();
		nl();
		echo();
		};
	} else {
		fprintf(stderr,"no mouse control found!\n");	
	};
 endwin();
}