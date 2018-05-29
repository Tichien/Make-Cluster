CC=gcc
CFLAGS=-std=c99
SRC=cluster.c cluster1.c cluster2.c cluster3.c cluster4.c cluster5.c cluster6.c cluster7.c cluster8.c cluster9.c cluster10.c cluster11.c cluster12.c cluster13.c cluster15.c cluster16.c cluster17.c cluster18.c cluster19.c cluster20.c cluster21.c 
OBJ=$(SRC:.c=.o) 

all: $(OBJ)

.SUFFIXES: .c .o

.c.o:
	$(CC) -c $(CFLAGS) $<; sleep 10

clean:
	rm -f $(OBJ) *~