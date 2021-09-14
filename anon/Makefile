

# all: anon_logs

anon_logs: anon_logs.c alist.o
	gcc -O3 anon_logs.c alist.o -o anon_logs

anonl_c: anonl.c alist.o
	gcc -O3 anonl.c alist.o -o anonl_c

anon_logs2: anon_logs2.c alist.o
	gcc -O3 anon_logs.c alist.o -o anon_logs2

alist.o: alist.h alist.c rb_tree.c avl_tree.c
	gcc -O3 alist.c -c -o alist.o

alist1.o: alist1.h alist1.c rb_tree1.c avl_tree1.c
	g++ -O3 alist1.c -c -o alist1.o

anon_logs_cc: anon_logs.cc alist1.o
#	g++ -fpermissive anon_logs.cc -o anon_logs1
	g++  -fpermissive -DUSE_STL=0 anon_logs.cc alist1.o -o anon_logs_cc

anon_logs_cc_stl: anon_logs.cc alist1.o
	g++  -fpermissive -DUSE_STL=1 anon_logs.cc -o anon_logs_cc_stl

anonl_cc_stl: anonl.cc 
#	g++ -std=c++11 -fpermissive -DUSE_STL=1 anonl.cc -o anonl_cc_stl
	g++ -std=c++11 -DUSE_STL=1 anonl.cc -o anonl_cc_stl


clean :
	rm -f *.o anon_logs anon_logs1

