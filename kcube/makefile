GTK_LIBS = `gtk-config --libs`
GTK_CFLAGS = `gtk-config --cflags`

list.o: list.c list.h
	gcc list.c -c -o list.o

listm.o: list.c list.h
	gcc list.c -c -o listm.o

#kcube: kcube.c list.o
#	gcc kcube.c -o kcube list.o  -L/usr/X11R6/lib -lGL -lGLU -lglut -lX11 -lXmu -lXi -lm

kcube: kcube.c list.o
	gcc kcube.c -o kcube list.o  -L/usr/X11R6/lib -lGL -lGLU -lglut -lX11  -lXi -lm

kcube1: kcube1.c list.o
	gcc kcube1.c -o kcube1 list.o  -L/usr/X11R6/lib -lGL -lGLU -lglut -lX11 -lXmu -lXi -lm

kcubem: kcubem.c listm.o
	gcc kcubem.c -o kcubem listm.o -framework GLUT -framework OpenGL -framework Cocoa

kcubema: kcubem.c listm.o
	clang kcubem.c -o kcubema listm.o -framework GLUT -framework OpenGL -framework Cocoa

clean:
	rm -f kcube1 kcube kcubem *.o