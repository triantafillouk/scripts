GTK_LIBS = `gtk-config --libs`
GTK_CFLAGS = `gtk-config --cflags`

all: kcube

list.o: list.c list.h
	gcc list.c -c -o list.o

listm.o: list.c list.h
	gcc list.c -c -o listm.o

#kcube: kcube.c list.o
#	gcc kcube.c -o kcube list.o  -L/usr/X11R6/lib -lGL -lGLU -lglut -lX11 -lXmu -lXi -lm

ifeq ($(UNAME), Darwin)
GFLAGS=-DDARWIN=1 -framework GLUT -framework OpenGL -framework Cocoa
else
GFLAGS=-DDARWIN=0 -L/usr/X11R6/lib -lGL -lGLU -lglut -lX11  -lXi -lm
endif

kcube: kcube.c list.o
	gcc kcube.c -o kcube list.o ${GFLAGS}

#all: kcube

clean:
	rm -f kcube1 kcube kcubem *.o