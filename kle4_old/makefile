# Make file

# Last modified 2020/09/10

# default values for flags
UNAME := $(shell uname)

PCURSES=0
XLIB=0
GTK=0
XPLOT=0
EMBED_ICONS=0

# include support for notes database in sqlite3
TNOTES=1

GTKINCLUDE3=`pkg-config gtk+-3.0 --cflags`
GTKINCLUDE3=`pkg-config gtk+-3.0 --cflags`
_X11_=0

OSYSTEM=OTHERUNIX
SHAREDDIR=/usr/share


# for Darwin
ifeq ($(UNAME), Darwin)
GLIBINCLUDE=`pkg-config glib-2.0 --cflags`
LPCURSES0=-lpanel -lncurses -L/opt/local/lib `pkg-config glib-2.0 --libs`
OSYSTEM=DARWIN
SHAREDDIR=/usr/local/share
X11include=-I/opt/X11/include/
X11lib0= -lX11 -L/opt/X11/lib  `pkg-config glib-2.0 --libs`
EXTFILE=.xe_ext_mac
WSL:=0
CC=clang
#CC=gcc-9
endif

# for Linux
ifeq ($(UNAME), Linux)
GLIBINCLUDE=`pkg-config glib-2.0 --cflags`
LPCURSES0=-lpanelw -lncursesw  `pkg-config glib-2.0 --libs`
OSYSTEM=LINUX
X11include=-I/opt/X11/include/
X11lib0=-L/usr/X11R6/lib -lX11 -L/opt/X11/lib  `pkg-config glib-2.0 --libs`
WSL:=`uname -a|grep "icrosoft" |wc -l`
EXTFILE:=.xe_ext_$(WSL)
CC=gcc -DWSL=$(WSL) 
endif

# for solaris
ifeq ($(UNAME), SunOS)
GLIBINCLUDE=-I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include
X11include=
X11lib0=-L/usr/X11R6/lib -lX11 -L/opt/X11/lib 
LPCURSES0=-lpanel -lcurses -L/usr/lib -lglib-2.0
OSYSTEM=SOLARIS
EXTFILE=.xe_ext_linux
WSL=0
CC=gcc
endif

# for cygwin
ifeq ($(UNAME), CYGWIN_NT-10.0)
GLIBINCLUDE=`pkg-config glib-2.0 --cflags`
LPCURSES0=-lpanelw -lncursesw  `pkg-config glib-2.0 --libs`
OSYSTEM=CYGWIN
WSL=0
##CC=gcc
endif

xe : XLIB=1
xe : _X11_=1
xe : PCURSES=0 

ge2c : XPLOT=1

emp  : PCURSES=1
em : PCURSES=1
em : TNOTES=0

ifeq ($(TNOTES),1)
SQLITE3=`pkg-config sqlite3 --libs`
endif

ifeq ($(TNOTES), 1)
LPCURSES=$(LPCURSES0) ${SQLITE3}
X11lib=$(X11lib0) ${SQLITE3}
else
LPCURSES=$(LPCURSES0)
X11lib=$(X11lib0)
endif

ifeq ($(CC), clang)
# the following is needed for clang compiler
CPU_OPTIONS= -O3  -I/usr/include/x86_64-linux-gnu -D$(OSYSTEM)=1
else 
CPU_OPTIONS= -O3  -D$(OSYSTEM)=1  
endif

ifeq ($(WSL), 1)
LIN:=microsoft
else
LIN:=native
endif


ge2c pixmp ge3c : GTK=1 

ge3c: GTK3=1
ge3c: XPLOT=1

ge2c ge3c : _X11_=1 

ge2c gldisplay.o : GTK2=1

FLAGS1 =  -DXLIB=$(XLIB) -D$(OSYSTEM)=1 -DWSL=$(WSL) -DPCURSES=$(PCURSES) -DTNOTES=$(TNOTES) -DGTK=$(GTK)  -DXPLOT=$(XPLOT) -DEMBED_ICONS=${EMBED_ICONS} -D_X11_=$(_X11_) -DTNOTES=$(TNOTES) $(X11include) -I/usr/include/ncurses $(GLIBINCLUDE)

FLAGS3 =  -DXLIB=$(XLIB) -D$(OSYSTEM)=1 -DXPLOT=$(XPLOT) -DEMBED_ICONS=${EMBED_ICONS} -D_X11_=$(_X11_) $(X11include) $(GLIBINCLUDE)

ge2c pixmp gdk2 : GTKINCLUDE=`pkg-config gtk+-2.0 --cflags` -DGTK2=1 

tplot c3 ge3 ge3c: GTKINCLUDE=`pkg-config gtk+-3.0 --cflags` -DGTK3=1 
tplot c3 ge3 ge3c: GTKINCLUDE3=`pkg-config gtk+-3.0 --cflags` -DGTK3=1 

ifeq ($(TNOTES), 1)
GTK2_FLAGS=`pkg-config gtk+-2.0 --libs` $(X11lib) ${SQLITE3}
GTK3_FLAGS=`pkg-config gtk+-3.0 --libs` $(X11lib) ${SQLITE3}
else
GTK2_FLAGS=`pkg-config gtk+-2.0 --libs` $(X11lib) 
GTK3_FLAGS=`pkg-config gtk+-3.0 --libs` $(X11lib) 
endif

xe.h : 
	echo cleaning all
	echo $(FLAGS1)
	rm -f *.o

.c.o : $o
	${CC} $(FLAGS1) -c -Wall $(CPU_OPTIONS) $(GTKINCLUDE) -funsigned-char -DTNOTES=$(TNOTES)  $*.c 
#	${CC}  -c -Wall -O2 -mcpu=pentium -I/usr/include/gtk-2.0 -I/usr/include/glib-1.2 -funsigned-char $*.c 

#	This for SCO. -J is for unsigned char
#	cc -c -J -O2 -b elf $*.c


test_scroll: tests/test_scroll.c
	${CC} tests/test_scroll.c -o test_scroll $(GTKINCLUDE3)  $(GTK3_FLAGS) -lm -lX11

test_dialogs: tests/test_dialogs.c
	${CC} tests/test_dialogs.c -o test_dialogs $(GTKINCLUDE3)  $(GTK3_FLAGS) -lm -lX11

test_gtk_about: tests/test_gtk_about.c
	${CC} tests/test_gtk_about.c -o test_gtk_about $(GTKINCLUDE3)  $(GTK3_FLAGS) -lm -lX11

test_view: tests/test_view.c
	${CC} tests/test_view.c -o test_view $(GTKINCLUDE3)  $(GTK3_FLAGS) -lm -lX11

#stroker: demos/stroker.c
#	gcc demos/stroker.c -o stroker `pkg-config gtk+-3.0 --cflags --libs`

#expose: demos/expose.c
#	gcc demos/expose.c -o expose `pkg-config gtk+-3.0 --cflags --libs`

#button_test : demos/button_test.c
#	gcc demos/button_test.c `pkg-config gtk+-3.0 --cflags --libs`  -o button_test

#listbox-dnd: demos/listbox-dnd.c
#	gcc demos/listbox-dnd.c -o listbox-dnd `pkg-config gtk+-3.0 --cflags --libs`

mouse_test: tests/mouse_test.c
	gcc tests/mouse_test.c $(FLAGS1) -I/usr/include/ncursesw -funsigned-char -o mouse_test ${LPCURSES} -lm

mouse_test1: tests/mouse_test1.c
	gcc tests/mouse_test1.c $(FLAGS1) -I/usr/include/ncursesw -funsigned-char -o mouse_test1 ${LPCURSES} -lm

mouse_test2: tests/mouse_test2.c
	${CC} tests/mouse_test2.c $(FLAGS1) -I/usr/include/ncursesw -funsigned-char -o mouse_test2 ${LPCURSES} -lm

all : cleanall emp ge3c ge2c xe

tests: test_scroll test_dialogs test_gtk_about test_view


