project : hlist.o vlist.o start.o
	cc hlist.o vlist.o start.o -lncurses -o project
hlist.o : hlist.c hlist.h
	cc -c hlist.c -Wall
vlist.o : vlist.c vlist.h
	cc -c vlist.c -Wall
start.o : start.c hlist.h
	cc -c start.c -Wall 
