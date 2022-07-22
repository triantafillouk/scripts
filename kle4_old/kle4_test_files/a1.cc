
yk"Û·ÂÒ'a1.cc"
# "ksd<>[]hfkhdsfhd" // kj<>[]{}shdfkjhdsfkh
# this hquote5 /* skdfsdfkjh */
	'kjshfdkdshf'
sdf"12345
    lsdjflks\"dflds fj 
	lskdfldskjfsdjf
Œ±Œ≤Œ≥Œ¥ŒµŒ∂
	"
·‚„‰ÂÊÁËÈÍÎÏÌÓÔÒÛÙıˆ˜¯˘
¡¬√ƒ≈∆«»… ÀÃÕŒœ–—”‘’÷◊ÿŸ

sdf
void updupd()
{
	register int i;
	for (i = 0; i < term.t_nrow; i++) {

		/* for each line that needs to be updated*/
		if ((vscreen[i]->v_flag & VFCHG) != 0) {
		sdf	updateline(i, vscreen[i]);
		}
	};
	show_slide();
//	return(TRUE);
}
sdf

; sdgdsfgsdfgdsg
! sdfgdsfgdsgf
% sdkjhfksjdfkjsfdj

! skdhfskdhf
 for 
 int
/*
;sfsdfgsdgf

sfdsffsg
456789012345 6 */
	fprintf("1234567virtual display screen, and the ones that make thevirtual display screen, and the ones that make the");
/*
 Monospace
 Corier New         a1.cc
	Œ±Œ≤Œ≥Œ¥ŒµŒ∂ Œ∑Œ∏ŒπŒ∫ŒªŒºŒΩŒæ ŒøœÄœÅœÉœÑœÖœÜ œáœàœâ ŒëŒíŒì ŒîŒïŒñŒóŒò ŒôŒöŒõŒúŒùŒû ŒüŒ†Œ°Œ£
	DejaVu Sans  Monon
 FreeMono
 Lucida Sans Typewriter
 * The functions in this file handle redisplay. There are two halves, the kasdkjhasdkahsdkjhdkas aksdh ashd hahds a lkjalfjasldjf ljasdfl jsladjfl asflkjadsflks jadfj
 * ones that update the virtual display screen, and the ones that make the
 * physicaL display screen the same as the virtual display screen. These   
 *  Fu nctions use hin          ts that are if left in the ^B windows by the commands.	   
·‚„‰ÂÊÁËÈÍÎÏÌÓÔÒÛÙıˆ˜¯˘
¡¬√ƒ≈∆«»… ÀÃÕŒœ–—”‘’÷◊ÿŸ
1=-100-0 ABwsdfsdf2331232 /*    */

·‚„‰ÂÊÁËÈÍÎÏÌÓÔÒÛÙıˆ˜¯˘
¡¬√ƒ≈∆«»… ÀÃÕŒœ–—”‘’÷◊ÿŸ

a1
a2
a3


a		"	wwww00023
				dsdfdasf"
 */
df
 "This is a world of misery in most places. Only a few countries
can give food to their people and correct medicin. This is going to be worst in the future
and poor counrties have no big means to change this trend.
    intthis"
// akjshdf kajshdjsadh 'kajshdfasdfh

#include    <stdio.h>
#include	"xe.h"
#include    "edef.h"
#include	"color.h"
	123
		456
//extern double l10(double);
int put_text(VIDEO *vp1,int row,int maxcol);
 /* external function definitions */
extern void xdisp_pokebox(int sy1,int sx1,int sy2,int sx2);
#if	XLIB
extern void hdab(int x,int b,char *s,int bcolor,int fcolor);
#endif
char *a="sdfsdf";

void vhdab(int x,int b,char *s,int bcolor, int fcolor);
extern void clear_hmenu();
void refresh_menu();

int lstart;	/* if updating the start spacing in line */
line 56
int c_sx,c_sy,c_maxl;	/* menu box */
MENUS *c_menu;		/* current menu */

#if	DATE
char *time2a();
#endif
static int hl=1;	/* Highlight on. It is ^f 1 if not on the modeline */

VIDEO    **vscreen;                  /* Virtual screen. */
VIDEO   **pscreen;                      /* Physical screen. */

'asdfasdfas' "djklf"
"sdlkjdsf lksjdf k"

#include   lksjdf

void clear_status();
void highlight_column_selection(WINDP *wp, LINE *lp, int i);
void vt1eol(int c,int color);
void updone(WINDP *wp);
void movecursor(int row, int col);
void modeline(WINDP *wp);
void updgar();
void show_cursor();
int getcline(int flag);
void updone(WINDP *wp);
void vtline(WINDP *wp, LINE *lp);
void updall(WINDP *wp);
void top_menu();
void updupd();
void status_line(char *);
void mlputs(char *s);
void highlight_selection(WINDP *wp, LINE *lp, int i);
//void showcpos();
void highlight(int c);
void updpos();
void update_column_selection(WINDP *wp, LINE *lp);

extern int show_slide();

/*
 * Initialize the data structures used by the display code. The edge vectors 
 * used to access the screens are set up. The operating system's terminal I/O
 * channel is set up. All the other things get initialized at compile time.
 * The original window has "WFCHG" set, so that it will get completely
 * redrawn on the first call to "update".
 */

void vtinit(int argc, char **argp)
{
    register int i;
    register VIDEO *vp;
    TTopen();		/* open the screen */
    TTkopen();		/* open the keyboard */
//	printf("vtinit: ---- rows=%d cols=%d\n",term.t_mrow,term.t_mcol);
    vscreen = (VIDEO **) malloc( (term.t_mrow+2)*sizeof(VIDEO *)); // 100
    if (vscreen == NULL) exit(1);
    pscreen = (VIDEO **) malloc( (term.t_mrow+2)*sizeof(VIDEO *));

    if (pscreen == NULL) exit(1);
    for (i = 0; i < term.t_mrow+2; i++)
        {
        vp = (VIDEO *) malloc(sizeof(VIDEO)+ term.t_mcol*sizeof(short)+2);
        if (vp == NULL)   exit(1);

	vp->v_flag = 0;
	vp->v_rfcolor = gfcolor;
	vp->v_rbcolor = gbcolor;

        vscreen[i] = vp;
        vp = (VIDEO *) malloc(sizeof(VIDEO)+term.t_mcol*sizeof(short)+2);

	if (vp == NULL)   exit(1);

	vp->v_flag = 0;
        pscreen[i] = vp;
        };
	if(toprow) { 
		top_menu();
		updateline(0,vscreen[0]);
	};
}

/*
 * Clean up the virtual terminal system, in anticipation for a return to the
 * operating system. Move down to the last line and clear it out (the next
 * system prompt will be written in the line). Shut down the channel to the
 * terminal.
 */
void vttidy()
{
    clear_status();
	if(vscreen!=NULL) {
    TTclose();
    TTkclose();
	};
}

/*
 * Set the virtual cursor to the specified row and column on the virtual
 * screen. There is no checking for nonsense values; this might be a good
 * idea during the early stages.
 */
void vtmove(int row,int col)
{
    vtrow = row;
    vtcol = col;
}

/* Write a character to the virtual screen. The virtual row and
   column are updated. If we are not yet on left edge, don't print
   it yet. If the line is too long change the color of the last letter.
   This routine only puts printing characters into the virtual
   terminal buffers. Only column overflow is checked.
*/

void vtputc(unsigned int c)
{
// int i;
	register unsigned int ctl;
	register VIDEO *vp;	/* ptr to line being updated */
	vp = vscreen[vtrow];
	ctl=0;
	if(syntaxh && hl) highlight(c); 

	if (c!='\t' && c!=' ') 	lstart=FALSE; 
	if (c == '\t') {
		do {
			vtputc(' ');
		} while (((vtcol + curwp->w_lcol)& vtab) != 0);
	}  else 
	if (((c < 0x20 ) || c == 0x7F) && hl ) 
	{
	  if(specialh) {
		if(c==BOLD) { ctl=0x100*(vp->v_rfcolor+0x10*vp->v_rbcolor);return;};
		/* ^W, ^L appears as an horizontal line if at start of line */
		/* It should check the end of line but this is more difficult*/
		if(c==23 && vtcol==0) { /* single line green */
			vt1eol(hline1,color[W_FORE]);return;
		} else if (c==12 && vtcol==0) { /* single line red */
			vt1eol(hline1,color[L_FORE]);return;
		};
	  };
	  /* other control characters */
		ctl = 0x100*(color[CTRLFORE]+0x10*vp->v_rbcolor);
		vp->v_text[vtcol++] = ctl + '^';
		vp->v_text[vtcol++] = ctl + (c^0x40);
		return;
	} else 

	/* there might be singularity at 155=128+27 */
	if (c==155) { 
		if( charset==L928 && ofont==ilang ) { 
			ctl = 0x100*(color[ORIZON]+0x10*vp->v_rbcolor);
			vp->v_text[vtcol++] = ctl + c-0x40;
		} else { vp->v_text[vtcol++]=0x100*(vp->v_rfcolor+0x10*vp->v_rbcolor)+'‰';}; // 200
	} else 
	{ /* all the rest characters */
		if(syntaxh) {
		   if(hl) {
			/* single quotes */
			if(hquote0) ctl=0x100*(color[SPECFORE]+0x10*vp->v_rbcolor);
			/* double quotes */
			if(hquote) if(c!=34 && !lstart) ctl = 0x100*(color[QUOTEFORE]+0x10*color[QUOTEBACK]);
			/* C,C++ Comments */
			if(hquote1 || hquote5) ctl = 0x100*(color[COMMENTFORE]+0x10*color[BACKGROUND]);
//			if(hquote5) ctl = 0x100*(color[COMMENTFORE]+0x10*color[BOLDBACK]);
			/* Bold */
			if(hquote2 && specialh) ctl = 0x100*(color[BOLDFORE]+0x10*color[BOLDBACK]);
			/* Preprocessing */
			if(hquote3 && !hquote1) ctl = 0x100*(color[PREPFORE]+0x10*vp->v_rbcolor);
			/* % tag */
			if(hquote4 && !hquote1) ctl = 0x100*(color[TAGFORE]+0x10*vp->v_rbcolor);
			if(hselection) ctl = 0x100*(color[FOREGROUND]+0x10*color[MODEBACK]);
		   };
		};
		if(hl) {
		if(ctl==0)
		switch(c) { 
			case 39: 
				ctl = 0x100*(color[SPECFORE]+0x10*vp->v_rbcolor);
				break;
			case '[': case ']':
			case '(': case ')':
			case '{': case '}':
`lfdjglksfg`;

			case ';': case ':':
				ctl = 0x100*(color[SPECFORE]+0x10*vp->v_rbcolor);
				break;
			case '*': 
				ctl = 0x100*(color[CNUMERIC]+0x10*vp->v_rbcolor);
				break;
			case '=':
			case '+': case '-':
			case '&':
			case '<': case '>': case ',':
			ctl = 0x100*(color[CNUMERIC]+0x10*vp->v_rbcolor);
			break;
			default:
			ctl=0x100*(vp->v_rfcolor+0x10*vp->v_rbcolor);
		};
		} else ctl=0x100*(vp->v_rfcolor+0x10*vp->v_rbcolor);
		/* if on the orizon make it a different color */
		if (((vtcol == term.t_ncol-1) && hl) || (vtcol==0 && curwp->w_lcol > 0)) 
			ctl = 0x100*(color[ORIZON]+0x10*(vp->v_rbcolor));
		/* If the font is not the same as the document char set
		   convert to the right language.
		   Then put the real character on the screen.
		*/
		vp->v_text[vtcol] = (ofont==ilang||c<128 || !hl) ? ctl+c : ctl+xlate(ilang,c);

		vtcol++;
	};
}

/*
 * Erase from the end of the software cursor to the end of the line on which 
 * the software cursor is located (space char and default colors).
 */
void vteeol(LINE *lp)
{
    register int ctl;
    register VIDEO  *vp;
	register char blank;
    vp = vscreen[vtrow];
	if(xwin) blank=' ';
	else blank='_'; 
    ctl=0x100*(vp->v_rfcolor+0x10*vp->v_rbcolor)+' ';

	if(region_mode==REGIONM_COLUMN) {
		if(cselection || lp==curwp->w_dotp) {
		    while (vtcol < term.t_ncol) {
				if( vtcol<col0-curwp->w_lcol || vtcol>col1-curwp->w_lcol) vp->v_text[vtcol++] = ctl;
				else vp->v_text[vtcol++] = 0x100*(vp->v_rfcolor+0x10*color[MODEBACK])+blank;
		   };
		} else {
	    	while (vtcol < term.t_ncol) {
 				vp->v_text[vtcol++] = ctl;
   			};
   		};

	} else {
	    while (vtcol < term.t_ncol) {
 			vp->v_text[vtcol++] = ctl;
	   };	
	}
}

/* fill the rest of the line with a char and foreground color */
void vt1eol(int c,int color)
{
    register int ctl;
    register VIDEO  *vp;
	register short int *vpt;
    vp = vscreen[vtrow];
	vpt = &vp->v_text[0];
    ctl=0x100*(color+0x10*vp->v_rbcolor)+c;
    while (vtcol <= term.t_ncol) { *vpt++ = ctl; vtcol++;};
}

/* upscreen:	user routine to force a screen update
		always finishes complete update		*/
int upscreen(int n)
{
	update(TRUE);
	return(TRUE);
}

/*
 * Make sure that the display is right. This is a three part process. First,
 * scan through all of the windows looking for dirty ones. Check the framing,
 * and refresh the screen. Second, make sure that "currow" and "curcol" are
 * correct for the current window. Third, make the virtual and physical
 * screens the same.
 */
int update(int force)
{
	register WINDP *wp;
//	int dummy;
	if (force == FALSE && kbdmode == PLAY)	return(TRUE);
	/* update any windows that need refreshing */
//	printf("update --- 1 force=%d\n",force);
	if(!in_menu && !entry_mode) { 
		hide_cursor();
//		printf("update: --- hide_cursor1 entry_mode=%d\n",entry_mode);
	};
	if(curwp==NULL) return(FALSE);
//	cselection=0;	// ???
	curwp->w_ecol = getccol();
	if(curwp->show_selection) {
		curwp->w_emarkp = curwp->w_dotp;
//		curwp->w_ecol = getccol();
//		printf("curcol = %d\n",curwp->w_ecol);
		if(region_mode == REGIONM_LINE) curwp->w_emarko= curwp->w_dotp->l_used;
		else	curwp->w_emarko = curwp->w_doto;
//		curwp->w_flag |= WFMOVE; //sgarbf = 1;
	};
//	printf("update: --- updpos\n");
	updpos();
	currow = fwl(curwp);
	wp = wheadp;
//	printf("update: --- to updall\n");
	while (wp != NULL) {
//		now_wp = wp;
		now_ht = wp->w_bufp->h_type;
		if (wp->w_flag || wp->show_selection) {
			/* if the window has changed, service it */
			reframe(wp); /* check the framing */
//			printf("update: --- reframe\n");
			if(wp==curwp) currow = fwl(curwp);
			if(curwp->show_selection) {
				wp->w_hquotes=0;
				updall(wp);
			} else {
			if ((wp->w_flag & ~WFMODE) == WFEDIT) {
				updone(wp);	/* update EDITed line */
			}
			else if ((wp->w_flag & ~WFMOVE ) || sgarbf!=FALSE ) {
				updall(wp);	/* update all lines */
			};
			};
//			printf("update: --- to modeline\n");
			if (wp->w_flag & WFMODE)
				modeline(wp);	/* update modeline */
			wp->w_flag = 0;
			wp->w_force = 0;
		}
		/* on to the next window */
		wp = wp->w_wndp;
	}
//	printf("update: --- to updgar\n");
	/* if screen is garbage, re-plot it */
	if (sgarbf != FALSE)  updgar();
//	printf("update: --- to updupd\n");
	/* update the virtual screen to the physical screen */
	updupd();

	/* update the cursor and flush the buffers */
	if(!entry_mode) {
//	movecursor(currow, getccol() - curwp->w_lcol); 
	movecursor(currow, curwp->w_ecol - curwp->w_lcol); 
//	printf("update: show_cursor2 row=%d ecol=%d lcol=%d\n",currow, curwp->w_ecol, curwp->w_lcol);
	if(!in_menu) {
//		printf("update: show_cursor2 entry_mode=%d\n",entry_mode);
		show_cursor();
	};
	TTflush();
	} else { 
//		printf("update: hide_cursor3 entry_mode=%d\n",entry_mode);
	//	hide_cursor();
	};
//	printf("titletext\n");
//	if(force) titletext();
	return(TRUE);
}
ËËËËË
/*	updone:	update the current line	to the virtual screen		*/

void updone(WINDP *wp) /* window to update current line in */
{
	register LINE *lp;	/* line to update */
	register int sline;	/* physical screen line to update */
//	register int i;
	/* search down the line we want */
//	printf("updone: --- 1\n");
	getwquotes(wp);
	fquote_state(wp->w_dotp, wp->w_linep ,wp);
	lp = wp->w_dotp;
	sline = fwl(wp);
	/* real window line is wp->w_topline + sline */
	/* and update the virtual line */
	vscreen[sline]->v_flag |= VFCHG;
	vtmove(sline, 0);
#if	0
	vscreen[sline]->v_rfcolor = wp->w_fcolor;
	vscreen[sline]->v_rbcolor = wp->w_bcolor;
#endif
//    if(region_mode==REGIONM_COLUMN) hselection=0;
	vtline(wp,lp);
	vteeol(lp); 
//	printf("updone: --- end\n");
}

int fwl(WINDP *wp)
{
 register int sline;
 register LINE *lp;
	lp = wp->w_linep;
	sline = wp->w_toprow;
	while (lp != wp->w_dotp) {
		sline++;
		lp = lforw(lp);
	}
	return(sline);
}

/*	updall:	update all the lines in a window on the virtual screen */
void updall(WINDP *wp)	/* window to update lines in */
{
	register LINE *lp;	/* line to update */
	register int sline;	/* physical screen line to update */
//	register int i;
//	printf("updall: --- 1 ---\n");
	if(syntaxh) {
	if(wp->w_hquotes==0) { /* update quote state */
		init_highlight();
		fquote_state(wp->w_linep, lforw(wp->w_bufp->b_linep),wp );
		setwquotes(wp);
	} else {
	
	  getwquotes(wp);

	  if(wp->w_linep != wp->w_hqline) {
		if(wp->w_hquotes==1) {	/* move back */
			bquote_state(wp);
			setwquotes(wp);
		};
		if(wp->w_hquotes==2) { /* move forward */
			fquote_state(wp->w_linep, wp->w_hqline, wp);
			setwquotes(wp);
		};
	   };

	}
	};
	/* search down the lines, updating them */
	lp = wp->w_linep;
	sline = wp->w_toprow;
//	printf("updall: --- 4 top=%d ntrows=%d\n",wp->w_toprow , wp->w_ntrows);
	while (sline < wp->w_toprow + wp->w_ntrows) {

		/* and update the virtual line */
		vscreen[sline]->v_flag |= VFCHG;
		vtmove(sline, 0);

		vscreen[sline]->v_rfcolor = wp->w_fcolor;
		vscreen[sline]->v_rbcolor = wp->w_bcolor;

		if (lp != wp->w_bufp->b_linep) {

		/* if we are not at the end */
			if(region_mode==REGIONM_COLUMN) hselection=0;
			vtline(wp,lp);
			// we must update the column selection here 
			if(wp->show_selection) {
			if(region_mode==REGIONM_COLUMN) {
				update_column_selection(wp,lp);
			} 
			};
			vteeol(lp);
			lp = lforw(lp);
		} else vteeol(lp);
		
		/* on to the next one */
		sline++;
	}
//	printf("updall: --- 5\n");
}

/*	updpos:	update the position of the hardware cursor and handle extended
		lines. This is the only update for simple moves.	*/
void updpos()
{
 int mo;
/*	currow = fwl(curwp); */	/* find the current row */
//	curcol = getccol();	/* find the current column */
//	printf("updpos ---- 1 ----\n");
	curcol = curwp->w_ecol;
	if(curcol < (curwp->w_lcol) || curcol+1 > curwp->w_lcol+term.t_ncol) {
		if(curcol < term.t_ncol) curwp->w_lcol=0;
		else {
			curwp->w_lcol=getrcol(0,getlcol(curwp->w_doto, HSCROLL)) ;
		};

		if((mo=curwp->w_lcol %8)!=0) curwp->w_lcol += 8-mo;
		curwp->w_flag |= WFHARD;
		curwp->w_flag &= ~WFFORCE;
	}
}


/*	updgar:	if the screen is garbage, clear the physical screen and
		the virtual screen and force a full update		*/
void updgar()
{
	register short *txt;
	register int i;
	register int j;
	register short ctl;
//	printf("updgar: ---- 1 ---\n");
	ctl = 0x100*(gfcolor+0x10*gbcolor)+' ';
	for (i = 0; i < term.t_nrow; i++) {
		/* flag line as changed */
		vscreen[i]->v_flag |= VFCHG;
		/* clear the physical line with current color */
		txt = pscreen[i]->v_text;
		for (j = 0; j <= term.t_ncol; j++) {
			txt[j] = ctl;
		};
	};
	if(sgarbff) {
	drv_color(color[0],color[1]);
	TTmove(0, 0);		 /* Erase the real screen. */
	TTeeop();
	};
	sgarbf = FALSE;			 /* Erase-page clears */
					 /* the message area. */
	clear_status();			/* needs to be cleared if colored */
}

 /* We must call this for every viewable buffer switch KT ???? */
 /* A different menu for each type of buffer=normal,dir,form */
extern void dab(int,int,int, char *,int,int);

void top_menu()
{
 int i;
 int fch,bch;
 MENUS *mm1;
 mm1 = &m_topn;
	// clear the area
	clear_hmenu();
	bch=color[CBOXTBACK]; /* 3 baground */
	fch=color[CBOXTFORE];  /* 7 foreground */
	
 for(i=0;i< mm1->num_elements;i++) {
	vhdab(mm1->element[i].menu->xpos, mm1->element[i].high, mm1->element[i].txt,bch,fch);
 };
}

/*	updupd:	update the physical screen from the virtual screen	*/


/*
 * Update a single line on the screen. 
 */

int updateline(int row,struct VIDEO *vp1)
/* row of screen to update */
/* virtual screen image */
{
 register int i,i1,j;
 register unsigned char c;
 register int l;
 int bcol;
 int ct1;
 /* highlight search pattern   */
 if(row>0) { /* Not on menu */
 /* match pattern highlight */
 if((l=strlen(pat))>0) {
  for(i=0,i1=0;i<=term.t_ncol;i++) {
    c = vp1->v_text[i] % 0x100;
    bcol = vp1->v_text[i]/0x100;bcol /= 0x10;
    if(bcol==color[MODEBACK]) continue;
    if( eq(c,pat[i1])) {
  	i1++;
  	if(i1 == l) {
  		/* pattern matched !! */
		ct1 = 0x100*(color[SEARFORE]+0x10*color[SEARBACK]);
		for(j=i-i1+1;j<=i;j++) 
			vp1->v_text[j] = ct1 + vp1->v_text[j]%256;
			i1=0;
  	}
    } else i1=0;
  };
 };
 };
#if	LINUX
	put_text(vp1,row,term.t_ncol-1); 
#endif
	vp1->v_flag &= ~VFCHG;
	return(TRUE);'asdfasdf'
}

/*
 * Redisplay the mode line for the window pointed to by the "wp". This is the
 * only routine that has any idea of how the modeline is formatted. You can
 * change the modeline format by hacking at this routine. Called by "update"
 * any time there is a dirty window.
 */
void modeline(WINDP *wp)
{
    register char *cp;
    register int c;
    register int n;		/* cursor position count */
    register BUFFER *bp;
    register int i;			/* loop index */
    char lchar;		/* character to draw line in buffer with */
	register unsigned int max_t;
	max_t = term.t_ncol - 25;
    hl=0; /* No highlighting on the mode lines */
    n = wp->w_toprow+wp->w_ntrows;      	/* Location. */
//	printf("modeline: --- max_t = %d wp->w_ntrows=%d\n",max_t, wp->w_ntrows);
    vscreen[n]->v_flag |= VFCHG ;/* Redraw next time. */

    vtmove(n, 0);                       	/* Seek to right line. */

    vscreen[n]->v_rfcolor = color[MODEFORE]; 
    vscreen[n]->v_rbcolor = color[MODEBACK]; 

	lchar = (wp == curwp) ? hline2 : hline1;	/* mark the current buffer */

    vtputc(lchar);
    bp = wp->w_bufp;
	n=1;

    /* display the modes */

	for (i = 1; i < 8 ; i++)	/* add in the state flags */
		if (wp->w_bufp->b_flag & (1 << i)) {
			vtputc(statecode[i]);n++;
		};
    vtputc(lchar);n++;
	for (i = 0; i < 8 ; i++)	/* add in the mode flags */
		if (wp->w_bufp->b_mode & (1 << i)) {
			vtputc(modecode[i]);n++;
		};
    vtputc(lchar);
    vtputc(lchar);
    vtputc(' ');
    n += 3;

	/* directory name */
#if	T103
    if( bp->b_fname[0]!='[' )
#else
    if( bp->b_bname[0]!='[' )
#endif
	{
	 if( bp->b_fname[0]!='/') {
		if(strlen(bp->b_dname) + strlen(bp->b_fname)<max_t) { 
		 // if there is enough space
 		 cp = &bp->b_dname[0];
		 if(strlen(cp)>max_t) cp = cp + (strlen(cp) - max_t);
		 if(strlen(cp)>35 && (bp->b_flag & BSVIEW)) cp = cp + strlen(cp) -35;
		 	for(;(c= *cp++);n++) vtputc(c);
		} else {
			vtputc('.');n++;
		};
		if(bp->b_fname[0]==0)
		{
			vtputc('{');n++;
#if	T103
			cp = &bp->b_fname[0];
#else
			cp = &bp->b_bname[0];
#endif
			for(;(c= *cp++);n++) vtputc(c);
			vtputc('}');n++;
		} else {
			vtputc('/');n++;
		};
	 };
    } else {
		if( bp->b_flag & BSDIRED) {
	 		cp = &bp->b_dname[0];
			if(strlen(cp)+strlen(bp->b_fname) < max_t  || bp->b_fname[0]==0 ) 
			{
			if(strlen(cp)>max_t) cp = cp + (strlen(cp) - max_t);
			for(;(c= *cp++);n++) vtputc(c);
			} else {
				vtputc('.');n++;
			};
			vtputc('/');n++;

		}
#if	!T107 
		else {	 
#if	T103
	 		cp = &bp->b_fname[0];
#else
	 		cp = &bp->b_bname[0];
#endif
			if(strlen(cp)+strlen(bp->b_fname) < max_t) {
			if(strlen(cp)>max_t) cp = cp + (strlen(cp) - max_t);
			if(strlen(cp)>35 && (bp->b_flag & BSVIEW)) cp = cp + strlen(cp) -35;
			for(;(c= *cp++);n++) vtputc(c);
			} else {
				vtputc('.');n++;
			};
		};
#endif	
	};

    if (bp->b_fname[0] != 0)            /* File name. */
    {
        cp = &bp->b_fname[0];
		for(;(c= *cp++);n++) {
			if(n>term.t_ncol-20) break;
			vtputc(c);
		};
	
    	vtputc(' ');n++;
    }
    while (n++ <= term.t_ncol)             /* Pad to full width. */
    {
		vtputc(lchar); 
    }
    hl=1; /* end of modeline */
//	printf("modeline: --- to titletext\n");
	titletext();
}

void upmode()	/* update all the mode,menu lines */
{
	register WINDP *wp;
	wp = wheadp;
	while (wp != NULL) {
		wp->w_flag |= WFMODE;
		wp = wp->w_wndp;
	}
	top_menu();
//	titletext();	
}

/*
 * Send a command to the terminal to move the hardware cursor to row "row"
 * and column "col". The row and column arguments are origin 0. Optimize out
 * random calls. Update "ttrow" and "ttcol".
 */ 
void movecursor(int row, int col)
{
    if (row!=ttrow || col!=ttcol)
	{
        ttrow = row;
        ttcol = col;
	};
    if(row<term.t_mrow-1) {
	showcpos();
	};
    TTmove(row, col);
}

/*
 * Erase the message line. This is a special routine because the message line
 * is not considered to be part of the virtual screen. It always works
 * immediately; the terminal buffer is flushed via a call to the flusher.
 */
void clear_status()
{
 if(entry_mode) {
// 	printf("status_line entry %d [%s]\n",e_pos,e_string);
	status_line(e_string);
	show_cursor_dl(e_pos); 
	// we must write here the contents of getstring
	return;
 };

#if	DATE
	status_line(time2a());
#else
	status_line("");
#endif
//	printf("clear_status: show_cursor\n");
#if	SLOWDATE
	mpresf = FALSE;
#endif
}


 /* update line starting at offset start of specific window */
void vtline(WINDP *wp, LINE *lp)
{
 int start;
 register int c, i, col,llen;
 struct VIDEO *vp;

 register int i0;
 register int ht,ct0,ct1,ct2;
 int j,i1,c1,i2,bcol;
 char **ww,**ww1;
// printf("vtline: --- 1 ---\n");
 i1=0;
 vp=vscreen[vtrow];
 start = wp->w_lcol;
 hquote5=0;hquote3=0;hquote4=0;		/* initialize line depending quoting */
 hquote0=0;
 prev_slash=FALSE;prev_ast=FALSE;
 ht=wp->w_bufp->h_type;
 /* the following could be defined once for each window or just once per color scheme */
 ct0=((color[FOREGROUND]+0x10*color[BACKGROUND]));		/* normal type */
 ct1 = 0x100*(color[WORD1FORE]+0x10*color[BACKGROUND]);	/* word type 1 */
 ct2 = 0x100*(color[WORD2FORE]+0x10*color[BACKGROUND]);	/* word type 2 */

 if(ht==H_GTEXT || ht==H_SHELL || ht==H_TEXT) hquote=0;

 col = 0;
	lstart= TRUE;
	llen = llength(lp);
//	printf("llen=%d\n",llen);
	if(llen==0) {
		if(wp->show_selection) { /* mark is defined */
			if(region_mode != REGIONM_COLUMN) {
			if(lp==wp->w_smarkp) { hselection=(hselection)? 0:1;};
			if(lp==wp->w_emarkp) { hselection=(hselection)? 0:1;};
			};
		};
		return;
	};
	for (i=0;  col<start && i<llen; ++i) {
    	c = lgetc(lp, i);
        if (c == '\t') col |= vtab;
        else if ((c < 0x20 ) || c == 0x7F) ++col;
        ++col;
		if (c==BOLD && specialh) col-=2;
		// printf("~");
		if(syntaxh) highlight(c);
	};
		if(wp->show_selection) { /* mark is defined ,ok!*/
			if(region_mode != REGIONM_COLUMN) {
			if(lp==wp->w_smarkp && wp->w_smarko<start) { hselection=(hselection)? 0:1;};
			if(lp==wp->w_emarkp && wp->w_emarko<start) { hselection=(hselection)? 0:1;};
			}
		};
	if(i>=llen) {
		//if(
		return;
	};
	for (; i < llen && vtcol<=term.t_mcol; i++) {
		highlight_selection(wp,lp,i);
		vtputc(lgetc(lp, i));
	};
//	if(cselection) hselection=0;
	//if(llen>0)
	 highlight_selection(wp,lp,i);

	/*********** word highlight ***********/
	{
		if( ht != H_DOT && ht!=H_TEXT && ht!=H_GTEXT ) {
		 ww = wp->w_bufp->w0; 
		 ww1 = wp->w_bufp->w1; 
		 for(i0=0,i1=0;i0<=term.t_ncol;i0++) {
			 c1 = vp->v_text[i0] % 0x100;
			 bcol = vp->v_text[i0]/0x100;
			 if(bcol!=ct0) {i1=0;continue;};
			 if(notcletter(c1)) { i1=0;continue;};
			 if(i1==0) 
				for(j=0;j<50;j++) { /* should depend of the document language */
				 	i2=i0;c=c1;
					if(ww[j][0]==0) break;
				 	while (c==ww[j][i1]) {
						i1++;
						i2++;
						if(ww[j][i1]==0) break;
						c=vp->v_text[i2] % 0x100;
					};
					if((unsigned)(i2-i0) ==  strlen(ww[j]) ) {
						c=vp->v_text[i2] % 0x100;
						if(notcletter(c) || i2==llen ) { /* if the following char is not a letter */
							i0=i2-1;
							for(j=i0-i1+1;j<=i0;j++) 
								vp->v_text[j] = ct1 + vp->v_text[j]%256;
							i1=0;
				 			i0++;
							break;
						};
			 			break;	
					} else {
			 			i1=0;
					};
	 			};

				if(i1==0)
				 for(j=0;j<20;j++) { /* should depend of the document language */
	 				i2=i0;c=c1;
					if(ww1[j][0]==0) {i1=1;break;};
	 				while (c==ww1[j][i1]) {
						i1++;
						i2++;
						if(ww1[j][i1]==0) break;
						c=vp->v_text[i2] % 0x100;
					};
					if((unsigned)(i2-i0) ==  strlen(ww1[j]) ) {
						c=vp->v_text[i2] % 0x100;
						if(notcletter(c) || i2==llen) {
							i0=i2-1;
							for(j=i0-i1+1;j<=i0;j++) 
							vp->v_text[j] = ct2 + vp->v_text[j]%256;
						};
						i1=0;
						break;	
					};
					i1=0;
	 			};
 			};
 		};
	};
	/**************************************/
	if(wp->show_selection) { 
		if(region_mode != REGIONM_COLUMN) {
		if(lp==wp->w_smarkp && wp->w_smarko>i) { hselection=(hselection)?0:1;};
		if(lp==wp->w_emarkp && wp->w_emarko>i) { hselection=(hselection)?0:1;};
		}
	};
	
	if(syntaxh) for (; i<llen ; i++) {
	  // printf("@");
	  highlight(lgetc(lp,i));
	}
}

void update_column_selection(WINDP *wp, LINE *lp)
{
 int start;
 register int c, i,llen,ctl;
// int col;
 struct VIDEO *vp;

 start = wp->w_lcol;
 
 if(wp->w_ecol > wp->w_scol) { 
 	col0 = wp->w_scol;
	col1 = wp->w_ecol;
 } else {
 	col0 = wp->w_ecol;
	col1 = wp->w_scol;
 };
 vp=vscreen[vtrow];

 hselection=0;
	llen = llength(lp);
	for (i=0;  i<start && i<llen; ++i) {
		highlight_column_selection(wp,lp,i);
	};
	i=start;
//	if(llen>start) printf("--> i=%d start=%d llen=%d\n",i,start,llen);
	for (; /* i < llen && */ i< term.t_mcol+start; i++) {
		highlight_column_selection(wp,lp,i); // hselection=1;
		if(hselection) { ctl = 0x100*(color[FOREGROUND]+0x10*color[MODEBACK]);
//		printf("hs start=%d\n",start);
		c=vp->v_text[i-start] % 256;
		vp->v_text[i-start] =  ctl+c ;
		};
	};
	 highlight_column_selection(wp,lp,i);
}

void highlight_selection(WINDP *wp, LINE *lp, int i)
{
	if(wp->show_selection==0) return;
	if(region_mode == REGIONM_COLUMN) return;
	if(lp==wp->w_smarkp && i==wp->w_smarko) { hselection=(hselection)?0:1;};
	if(lp==wp->w_emarkp && i==wp->w_emarko) { hselection=(hselection)?0:1;};
}

void highlight_column_selection(WINDP *wp, LINE *lp, int i)
{
	if(lp==wp->w_smarkp && i==wp->w_smarko) { cselection=(cselection)?0:1;};
	if(lp==wp->w_emarkp && i==wp->w_emarko) { cselection=(cselection)?0:1;};
	if(cselection) { hselection=1;} else hselection=0;
	if(lp==wp->w_smarkp || lp==wp->w_emarkp) hselection=1;

	if(i< col0) { hselection=0;return;};
	if(i> col1) { hselection=0;return;};

}

/*
 * Display the current position of the cursor
 * This is on top of modeline !
 */
int showcpos()
{
#if	SHOWRPOS
	int col;
//	col = getccol()+1;
	col = curwp->w_ecol + 1;
	TTmove(curwp->w_toprow+curwp->w_ntrows,term.t_ncol-16);
#else
	TTmove(curwp->w_toprow+curwp->w_ntrows,term.t_ncol-12);
#endif
	drv_color(color[DROWCOL],color[MODEBACK]);
	TTflush();
#if	SHOWRPOS
	sprintf(pline,"%5u %3u %3u",getcline(0),curwp->w_doto+1,col);
#else
	sprintf(pline,"%5u %3u",getcline(0),curwp->w_doto+1);
#endif
	put_string(pline);
	return TRUE;
}

#if	UNDO
/* this is used only in undo functions ? */
long getlinenum(BUFFER *bp, LINE *sline)	/* get the a line number */
{
	register LINE	*lp;	/* current line */
	register long	numlines;	/* # of lines before point */

	/* starting at the beginning of the buffer */
	lp = lforw(bp->b_linep);

	/* start counting lines */
	numlines = 0L;
	while (lp != bp->b_linep)
		{
		/* if we are on the current line, record it */
		if (lp == sline)
			break;
		++numlines;
		lp = lforw(lp);
		};
	/* and return the resulting count */
	return(numlines + 1L);
}
#endif

int getcline(int flag)	/* get the current line number */
{
	static LINE *cwtl;	/* current window top line */
	static int wnum0;	/* current window top line number */

	register LINE *lp;
	register int numlines;
#if	0
	   	if(curbp == NULL) return(0);
		if(curbp->b_linep == NULL) return(0);
#endif
	numlines=0;
	if(cwtl != curwp->w_linep)
	   { /* we changed window or buffer */
	        lp = lforw(curbp->b_linep);
        	for (numlines=1; lp!=curwp->w_linep;numlines++) {
			lp = lforw(lp);
        	};
	  	wnum0 = numlines;
	  	cwtl = curwp->w_linep;
	  };

	if(flag) return(wnum0);

	lp = curwp->w_linep;
    for (numlines=wnum0;lp != curbp->b_linep && lp!=curwp->w_dotp;numlines++) {
		lp = lforw(lp);
    };

	/* and return the resulting count */
	return(numlines);
}

/*	1	2	3	4	5	6	7	8	9	0	1	2	3	4	5	6	7	8	9   */
	/* ksd fkjhf ks fkjdh fsdh fkdhfkjf sk kjd fhfkjdh kdhfksd ksdfh k sksfdhsd fksdf		sdhf skd fsd fsfh		hs ksdf sfksdf hsdf */
/*------------------------------------------*---j------------------------------8--------*9---------|---------1---------2---------3---------4---|-----5------*/
/*------------------------------------------*-------------------------------------------f---------------s---------------h---------------h-------------------*/

 /* Return current column.  Stop at first non-blank given TRUE argument. */
int getccol()
{
        register int c, i, col;
        col = 0;
        for (i=0; i<curwp->w_doto; ++i) {
                c = lgetc(curwp->w_dotp, i);
                if (c == '\t')
                        col |= vtab;
                else if (/* c<15 || (*/ c < 0x20 /* && c>22)*/ || c == 0x7F)
                        ++col;
                ++col;
		if(c==BOLD && specialh) col-=2;
        }
        return(col);
}

 /* Return real len. from lcol -> rcol   */
int getrcol(int lcol,int rcol)
{
        register int c, i, col;
	col=0;
        for (i=lcol; i<rcol; ++i) {
                c = lgetc(curwp->w_dotp, i);
                if (c == '\t')
                        col |= vtab;
                else if (/* c<15 || (*/ c < 0x20 /* && c>22)*/ || c == 0x7F)
                        ++col;
                ++col;
		if(c==BOLD && specialh) col-=2;
        };
	
        return(col);
}

int getlcol(int rcol,int len)
{
    register int c, i, col;
	col=0;
	if(len > term.t_ncol) len=term.t_ncol/2;
        for (i=rcol-1; i>=0; i--) {
                c = lgetc(curwp->w_dotp, i);
                if (c == '\t')  col |= vtab;
                else if (/* c<15 || (*/ c < 0x20 /* && c>22)*/ || c == 0x7F)
                        ++col;
                ++col;
		if(c==BOLD && specialh) col-=2;
		if(col>=len) break;
        };
	
        return(i);
}

/*
 * Set current column to position pos.
 * Used to set variable ccolumne
 */
int setccol(int pos)
{
    register int c;		/* character being scanned */
	register int i;		/* index into current line */
	register int col;	/* current cursor column   */
	register int llen;	/* length of line in bytes */

	col = 0;
	llen = llength(curwp->w_dotp);

	/* scan the line until we are at or past the target column */
	for (i = 0; i < llen; ++i) {
		/* upon reaching the target, drop out */
		if (col >= pos)	break;

		/* advance one character */
                c = lgetc(curwp->w_dotp, i);
                if (c == '\t')
                        col |= vtab;
                else if (/* c<15 || (*/ c < 0x20 /* && c>22) */ || c == 0x7F)
                        ++col;
                ++col;
		if(c==BOLD && specialh) col-=2;
        }

	/* set us at the new position */
	curwp->w_doto = i;

	/* and tell weather we made it */
	return(col >= pos);
}

void disp_pokebox(int sy1,int sx1,int sy2,int sx2)
{
 int x,y;
 unsigned short int c1,*text;
 int fc,bc;
// printf("disp_pokebox\n");
 if(sy2+1>term.t_nrow) sy2=term.t_nrow;
 for(y=sy1;y<sy2+1;y++) {
    TTmove(y,sx1);
    text=vscreen[y]->v_text+sx1;
    for(x=sx1;x<sx2+1;x++) {
     c1=*text % 256;
     fc=(*text/256) % 16;
     bc=(*text/256) / 16;
     drv_color(fc,bc); 
     if(c1==0x9b) TTputc(' ');	/* 0x9b does not redraw correctly here */
     else TTputc(c1);
     text++;
    };
 };
 clear_status();
}

void update_box(int sy1,int sx1,int sy2,int sx2)
{
 int x,y;
 unsigned short int c1,*text;
 int fc,bc;
// printf("update box %d %d %d %d\n",sx1,sy1,sx2,sy2);
 if(sy2+1>term.t_nrow) sy2=term.t_nrow;
 for(y=sy1;y<sy2+1;y++) {
    TTmove(y,sx1);
    text=vscreen[y]->v_text+sx1;
    for(x=sx1;x<sx2+1;x++) {
     c1=*text % 256;
     fc=(*text/256) % 16;
     bc=(*text/256) / 16;
     drv_color(fc,bc); 
     if(c1==0x9b) TTputc(' ');	/* 0x9b does not redraw correctly here */
     else TTputc(c1);
     text++;
    };
 };
}

int deskey(int n)	/* describe the command for a certain key */
{
	register int c;		/* key to describe */
	register char *ptr;	/* string pointer to scan output strings */
	char outseq[NSTRING];	/* output buffer for command sequence */
	int (*getbind())();

	/* prompt the user to type us a key to describe */
	status_line(": describe-key ");

	/* get the command sequence to describe
	   change it to something we can print as well */
	cmdstr(c = getckey(), &outseq[0]);
	strcat(outseq," =  ");
	/* find the right ->function */
	if ((ptr = getfname(getbind(c,1))) == NULL) {
		if(subname[0]!=0) ptr=subname;
		else  { ptr = "Not Bound"; return FALSE;};
	};
	strcat(outseq,ptr);
	status_line(outseq);
 return TRUE;
}


/* from random.c */
 /* define x-offset */
#define xof 1

int (*get_menucmd(MENUS *m1,int first))()
{
 int i,j,c,iol=0;
 int sx,sy,nu,maxl;
 int ftime;
 int orient;
 int bch=color[CBOXTBACK]; /* 3 baground */
 int fch=color[CBOXTFORE];  /* 7 foreground */
 int (*execf)();

 sx=sy=maxl=nu=0;
 in_menu=1;
 c_menu=m1;
 ftime=1;
 orient=m1->orient;
 nu = m1->num_elements;
 for(i=0;i<nu;i++) if((j=strlen(m1->element[i].txt)) >maxl) maxl=j;
 sx = m1->xpos; sy=m1->ypos;
 c_sx=sx;c_sy=sy;c_maxl=maxl;	/* export for mouse convenience */
 hide_cursor();
// printf("menu:hide_cursor orient=%d\n",orient);
 if(orient==VERTICAL) {
	drv_color(color[CBOXSFORE],color[CBOXSBACK]);
	disp_box(1,0*16+2,sy,sx,sy+nu+1,sx+maxl+1);
  } else {
	maxl++;
  };
#if	!TOPROW
 if(orient==HORIZONTAL) {
   TTmove(m1->xpos,m1->ypos);
   drv_color(color[CBOXTFORE],color[CBOXTBACK]);
   sprintf(pline,"%*s",term.t_ncol+1,"............. ");
   put_string(pline);
 };
 TTflush();
#endif

 /* display menu */
 for(i=0;i<nu;i++) {
	if(orient==VERTICAL) 
#if	XLIB
		{
		xdab(0,i+1,m1->element[i].high,m1->element[i].txt,bch,fch);
		}
#else
		dab(sx+xof,sy+i+1,m1->element[i].high,m1->element[i].txt,bch,fch);
#endif
	else	hdab(sx+maxl*(i),m1->element[i].high,m1->element[i].txt,bch,fch);
 };
 TTflush();
// i=iol=0; /* highlight element menu */
 i=iol=first;
 if(orient==VERTICAL)
#if	XLIB
	xdab(0,i+1,m1->element[i].high,m1->element[i].txt,fch,bch);
#else
	dab(sx+xof,sy+i+1,m1->element[i].high,m1->element[i].txt,fch,bch);
#endif
 else	hdab(sx+maxl*(i),m1->element[i].high,m1->element[i].txt,fch,bch);

 TTflush();
 /* get keyboard */
 while(1) {
	if(orient==HORIZONTAL && ftime==1) {
		i=first;
		ftime=0;
		execf=newline;
	} else {
#if	XLIB
	xupdate_box();
#endif
	c=getcmd();
//	printf("menu char=%d\n",c);
	execf = getbind(c,0);
	if(execf==0) {
		c=normalize(c);
		for(j=0;j<nu;j++) {
		 if(c==m1->element[j].high
//			14/10/2001
//		 	||c==m1->element[j].high+32
//			problem with letters o,p
			) break;
		};
		if(j==nu) continue;	/* while loop */
		i=j;
		if( m1->element[i].type == MMENU) {
			execf = newline;
			if(orient==VERTICAL)
#if	XLIB
				xdab(0,iol+1,m1->element[iol].high,m1->element[iol].txt,bch,fch);
#else
				dab(sx+xof,sy+iol+1,m1->element[iol].high,m1->element[iol].txt,bch,fch);
#endif
			else	hdab(sx+maxl*(iol),m1->element[iol].high,m1->element[iol].txt,bch,fch);

			if(orient==VERTICAL)
#if	XLIB
				xdab(0,i+1,m1->element[i].high,m1->element[i].txt,fch,bch);
#else
				dab(sx+xof,sy+i+1,m1->element[i].high,m1->element[i].txt,fch,bch);
#endif
			else	hdab(sx+maxl*(i),m1->element[i].high,m1->element[i].txt,fch,bch);
			iol=i;
		} else { 
			execf = m1->element[i].func ;
			nnarg = (int)m1->element[i].menu;
		};

		if(execf!=newline) {
//		printf("menu exit orient=%d\n",orient);
		if(orient==VERTICAL) 
#if	XLIB
//			{ xdisp_pokebox(1,0,0,term.t_mcol); }
			{ xdisp_pokebox(1); }
#else
			{ disp_pokebox(sy,sx,sy+nu+1,sx+maxl+1);}
#endif
		else refresh_menu(); 

		if(orient==HORIZONTAL) {
			if(execf==backchar) {
				i--;if(i<0) i=nu-1;
				execf=newline;
			};
			if(execf==forwchar) {
				i++;if(i>nu-1) i=0;
				execf=newline;
			};
		};
		 return(execf);
		};

	};
	};
	if(execf!=0) {
	 if( execf == forwline && orient == HORIZONTAL) execf=newline;
	 if( execf == newline) { 
	  while(execf == newline) {
		if( m1->element[i].type == MMENU) {
			execf = get_menucmd(m1->element[i].menu,0) ;
		} else {
			execf = m1->element[i].func ;
			nnarg = (int)m1->element[i].menu;
		};

		if ( m1->element[i].type == MFUNCTION) {
			if(orient==VERTICAL) 
#if	XLIB
//				xdisp_pokebox(1,0,0,term.t_mcol);
				xdisp_pokebox(1);
#else
				disp_pokebox(sy,sx,sy+nu+1,sx+maxl+1);
#endif
			else refresh_menu();
			return(execf);
		};
		if ( execf==backline || execf==backchar) {
			i--; if(i<0) i=nu-1;
			execf=newline;
		};
		if (execf==forwline || execf==forwchar) {
			i++;if(i>nu-1) i=0;
			execf=newline;
		};
		if(execf != newline) {
			if(orient==VERTICAL)
#if	XLIB
//				xdisp_pokebox(sy,sx,sy+nu+1,sx+maxl+1);
				xdisp_pokebox(sy);
#else
				disp_pokebox(sy,sx,sy+nu+1,sx+maxl+1);
#endif
			else refresh_menu();
			return(execf);
		} else {	/* highlight correct menu element */
		if(orient==VERTICAL)
#if	XLIB
			xdab(0,iol+1,m1->element[iol].high,m1->element[iol].txt,bch,fch);
#else
			dab(sx+xof,sy+iol+1,m1->element[iol].high,m1->element[iol].txt,bch,fch);
#endif
		else	hdab(sx+maxl*(iol),m1->element[iol].high,m1->element[iol].txt,bch,fch);

		if(orient==VERTICAL)
#if	XLIB
			xdab(0,i+1,m1->element[i].high,m1->element[i].txt,fch,bch);
#else
			dab(sx+xof,sy+i+1,m1->element[i].high,m1->element[i].txt,fch,bch);
#endif
		else	hdab(sx+maxl*(i),m1->element[i].high,m1->element[i].txt,fch,bch);
		iol=i;

		};
	  };
	 };
	 if( execf == ctrlg) { 
		if(orient==VERTICAL)
#if	XLIB
//			xdisp_pokebox(sy,sx,sy+1+nu,sx+maxl+1);
			xdisp_pokebox(sy);
#else
			disp_pokebox(sy,sx,sy+1+nu,sx+maxl+1);
#endif
		else refresh_menu();
		return(NULL);
	 };
	 if(orient==VERTICAL) {
		if(execf==backchar || execf==forwchar) {
#if	XLIB
//			xdisp_pokebox(sy,sx,sy+1+nu,sx+maxl+1);
			xdisp_pokebox(sy);
#else
			disp_pokebox(sy,sx,sy+1+nu,sx+maxl+1);
#endif
			return(execf) ;
		};
		if(execf==backline) {i=iol-1;if(i<0) i=nu-1;};
		if(execf==forwline) {i=iol+1;if(i>nu-1) i=0;};
#if	XLIB
	if(orient==VERTICAL) {
		xdab(0,iol+1,m1->element[iol].high,m1->element[iol].txt,bch,fch);
		xdab(0,i+1,m1->element[i].high,m1->element[i].txt,fch,bch);
	} else {
		dab(sx+xof,sy+iol+1,m1->element[iol].high,m1->element[iol].txt,bch,fch);
		dab(sx+xof,sy+i+1,m1->element[i].high,m1->element[i].txt,fch,bch);
	};
#else
	if(orient==VERTICAL) {
		dab(sx+xof,sy+iol+1,m1->element[iol].high,m1->element[iol].txt,bch,fch);
		dab(sx+xof,sy+i+1,m1->element[i].high,m1->element[i].txt,fch,bch);
	} else {
		hdab(sx+maxl*(iol),m1->element[iol].high,m1->element[iol].txt,fch,bch);
		hdab(sx+maxl*(i),m1->element[i].high,m1->element[i].txt,bch,fch);
	};
#endif
	 };
	iol=i;
	};
 };
}

/* display with attribute bold */
void dab(int x,int y,int b,char *s,int bcolor,int fcolor)
{
#if	XLIB
 hdab(x,b,s,bcolor,fcolor);return;
#endif
 TTmove(y,x);
 drv_color(fcolor,bcolor); 
 put_string(s);
 for(;*s;x++,s++) {
	if(b==*s) {
		TTmove(y,x);
		drv_color(fcolor+8,bcolor);
		TTputc(b);
		break;
	};
 };
}

int menu_command(int n)	/* execute menu */
{
 register int status=FALSE;
 register int (*execfunc)();		/* ptr to function to execute */
 int i,f1;
 MENUS *main_menu;
 main_menu=&m_topn;
 f1=0;
 for(i=0;i< main_menu->num_elements;i++) {
	if(main_menu->element[i].high==metal) { f1=i;break;};
 };
 if(f1> main_menu->num_elements-1) f1=main_menu->num_elements-1;
 execfunc = get_menucmd(main_menu,f1);
 in_menu=0;
 clear_status();
// if(execfunc != NULL) status=(*execfunc)(FALSE,0);
 if(execfunc != NULL) status=(*execfunc)(n);
 return(status);
}


/* m = array of elements
 * nu = number of elements
 * lmax = number of elements to show (height)
 * sx,sy = position
 * maxl = width 
 */
int selectl(char *title,char *m[],int nu,int lmax,int sx,int sy,int maxl)
{
 int i,c,iol,start;
 char name[80];
 int (*execf)();
 c_sx = sx;
 c_sy = sy;
 c_maxl = maxl;
#if	XLIB
// printf("selectl\n");
 if(!entry_mode) {
//	printf("selectl: hide cursor\n"); 
	hide_cursor();
 };
#else
 hide_cursor(); 
#endif
// printf("Start of select\n");// TTgetc(); 
 if(nu==0) { TTbeep();return(-1);};
 if(nu==1) return(0);
 if(!clexec) {
 start=0;
 in_menu=1;
 in_selection=1;
 if(nu<lmax) lmax=nu;
#if	GTK
 if(sy+lmax > 20) lmax=20-sy;
#else
 if(sy+lmax > term.t_nrow) lmax=term.t_nrow - sy;
#endif
/* disp_box(0,3,sy,sx,sy+lmax+1,sx+maxl+1); */
#if	!XLIB
 drv_color(color[CBOXSFORE],color[CBOXSBACK]);
#endif
 disp_box(0,16*7+2,sy,sx,sy+lmax+1,sx+maxl+1);
#if	!XLIB
 drv_color(color[CBOXTFORE],color[CBOXTBACK]);
#endif
// c=getcmd();
 start=disp_list(m,start,nu,lmax,sx,sy,maxl);
 i=0;
#if	XLIB
 put_string1(i,0,m[i],1);
#else
 drv_color(color[CBOXSFORE],color[CBOXSBACK]);
 TTmove(sy+1+i,sx+1);
 put_string(m[i]);
 drv_color(color[CBOXTFORE],color[CBOXTBACK]);
#endif
 iol=0;
 for(;;) {
#if	XLIB
	xupdate_box();
#endif
	c=getcmd();
	execf = getbind(c,0);
	if(execf == newline||execf==ctrlg||execf==yank ) break;
	if(execf == gotoline) { // get next character as number in list (xwin)
		i=get1key()-321;	// it works (321=256+65)
//		status_line1("xselect %d",i);
		execf = newline;
		break;
	};
	if(execf==gotobol) {
			start=0;
			start=disp_list(m,start,nu,lmax,sx,sy,maxl);
			i=0;
		} else
	if(execf==backline) {
		i=iol-1;
		if(i<0) {
			if(start>0)
			start=disp_list(m,start-1,nu,lmax,sx,sy,maxl);
			i++;
		};
	} else
	if(execf==backpage) {
			start-=lmax-1;
			if(start<0) start=0;
			start=disp_list(m,start,nu,lmax,sx,sy,maxl);
			i=0;
		} else
	if(execf==forwpage) {
			start+=lmax-1;
			if(start<0) start=0;
			start=disp_list(m,start,nu,lmax,sx,sy,maxl);
			i=0;
		} else
	if(execf==forwline) {
			i=iol+1;
			if(i==lmax) {
			start = disp_list(m,start+1,nu,lmax,sx,sy,maxl);
			i--;
			};
	}	else /* continue; */

	{
	 int j;
		if(c == (CNTRL | '[')) { execf=ctrlg;break;};
		/* if(c>='a' && c<='z') c-=32; */
		 c=capital(c);
		 for(j=start+i+1;j<nu;j++) {
		  if(c==m[j][0] || c==m[j][0]-32) {
			if(start+i<j && j<start+lmax){
				 i=j-start;
			};
			if(j>start+lmax-1) {
				start=j;
				start=disp_list(m,start,nu,lmax,sx,sy,maxl);
				i=j-start;
			};
			break;
		  };
	     }
	};
#if	XLIB
	put_string1(iol,0,m[iol+start],0);
#else
	TTmove(sy+1+iol,sx+1);
	put_string(m[iol+start]);
	drv_color(color[CBOXSFORE],color[CBOXSBACK]);
#endif
#if	XLIB
	put_string1(i,0,m[i+start],1);
#else
	TTmove(sy+1+i,sx+1);
	put_string(m[i+start]);
	drv_color(color[CBOXTFORE],color[CBOXTBACK]);
#endif
	iol=i;
	
 };
#if	XLIB
// xdisp_pokebox(sy,sx,sy+lmax+1,sx+maxl+1);
 xdisp_pokebox(sy);
 if(!entry_mode) {
//	printf("selectl:show cursor\n"); 
	show_cursor();
 };
#else
 disp_pokebox(sy,sx,sy+lmax+1,sx+maxl+1);
 show_cursor();
#endif
 in_menu=0;
 in_selection=0;
// printf("end selectl\n");
 if(execf == ctrlg ) return(-1);
 if(execf == yank ) return(-10);
 if(execf == newline ) return(i+start);
 } else {
	nextarg("",name,80);
 }
 return(-1);
}

int disp_list(char **m,int start,int nu,int lmax,int sx,int sy,int maxl)
{
 register int i;
 if(start+lmax>nu) start=nu-lmax;
// printf("disp list\n");
 for(i=start;i<start+lmax;i++) {
	sprintf(pline,"%-*s",maxl,m[i]);
#if	XLIB
	put_string1(i-start,0,pline,0);
#else
	TTmove(sy+1+i-start,sx+1);
	put_string(pline);
#endif
	};
 return(start);
}

/*
 * Quote the next character, and insert it into the buffer. All the characters
 * are taken literally, with the exception of the newline, which always has
 * its line splitting meaning. The character is always read, even if it is
 * inserted 0 times, for regularity. Bound to "C-Q"
 */
int quote(int n)
{
	register int    c;
	if(dont_edit()) return FALSE;
    if (n < 1)  return (FALSE);

    c = tgetc();

    return (linsert(n, c));
}


/*
 * Execute a macro.
 * The command argument is the number of times to loop. Quit as soon as a
 * command gets an error. Return TRUE if all ok, else FALSE.
 */
int exec_macro(int n)
{
 if (kbdmode != STOP) {
	status_line("Macro already active");
    return(FALSE);
 }
 if (n <= 0) return (TRUE);
	kbdrep = n;		/* remember how many times to execute */
	kbdmode = PLAY;		/* start us in play mode */
	kbdptr = &kbdm[0];	/*    at the beginning */
	return(TRUE);
}

/*
 * Initialize all of the buffers and windows. The buffer name is passed down
 * as an argument, because the main routine may have been told to read in a
 * file by default, and we want the buffer name to be right.
 */
void edinit(char *bname)
{
    register BUFFER *bp;
    register WINDP *wp=NULL;
	char *s1;
	gfcolor=color[FOREGROUND];
	gbcolor=color[BACKGROUND];

	s1=getcwd(start_dir,255);

	strcpy(current_dir,start_dir);
        bp = bfind(bname, TRUE, 0);             /* First buffer         */
        wp = (WINDP *) malloc(sizeof(WINDP)); /* First window         */
//		printf("edinit: --- 1\n");
        if (bp==NULL || wp==NULL)  exit(1);
        curbp  = bp;                            /* Make this current    */
        wheadp = wp;
        curwp  = wp;
        wp->w_wndp  = NULL;                     /* Initialize window    */
        wp->w_bufp  = bp;
        bp->b_nwnd  = 1;                        /* Displayed.           */
        wp->w_linep = bp->b_linep;
        wp->w_dotp  = bp->b_linep;
	wp->w_hqline = wp->w_linep; wp->w_hquotes =0;
        wp->w_doto  = 0;
        wp->w_markp = NULL;
        wp->w_marko = 0;
        wp->w_toprow = toprow;
	/* initalize colors to global defaults */
	wp->w_fcolor = gfcolor;
	wp->w_bcolor = gbcolor;
    wp->w_ntrows = term.t_nrow-1-toprow;    /* "-1" for mode line.  */
    wp->w_force = 0;
    wp->w_flag  = WFMODE|WFHARD;            /* Full.                */
}

-------------------------

