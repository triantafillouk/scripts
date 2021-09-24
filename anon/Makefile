

# all: anon_logs

anonl_c: anonl.c alist.o
	gcc -O3 anonl.c alist.o -o anonl_c

alist.o: alist.h alist.c rb_tree.c avl_tree.c
	gcc -O3 alist.c -c -o alist.o

alist1.o: alist1.h alist1.c rb_tree1.c avl_tree1.c
	g++ -O3 alist1.c -c -o alist1.o

anonl_cc: anonl.cc  alist1.o
	g++ -std=c++11 -O3 -DUSE_STL=0 anonl.cc  alist1.o -o anonl_cc

anonl_cc_stl: anonl.cc
	g++ -std=c++11 -O3 -DUSE_STL=1 anonl.cc -o anonl_cc_stl

anonl_cc_stl1: anonl.cc
	g++ -std=c++11 -O3 -DUSE_STL=1 -DUSE_GENERIC anonl.cc -o anonl_cc_stl1

clean :
	rm -f *.o anonl_cc anonl_c anonl_cc_stl anonl_cc_stl1

all: anonl_c anonl_cc anonl_cc_stl anonl_cc_stl1
	echo "make all"

run:
	./runall.sh

