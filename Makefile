project : hlist.o vlist.o main.o
	cc hlist.o vlist.o main.o -lncurses -o project
hlist.o : hlist.c hlist.h
	cc -c hlist.c -Wall
vlist.o : vlist.c vlist.h
	cc -c vlist.c -Wall
main.o : main.c hlist.h
	cc -c main.c -Wall 
