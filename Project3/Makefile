CC=gcc
CFLAGS=-g -O1 -Wall
LDLIBS=-lpthread

all: hostinfo client server 

client: client.c csapp.h csapp.c
server: server.c echo.c csapp.h csapp.c
hostinfo: hostinfo.c csapp.h csapp.c

clean:
	rm -f *.o *~ *.exe hostinfo client server csapp.o

