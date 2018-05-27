CC=gcc
CFLAGS=-std=c99
SRC=cluster.c cluster1.c cluster2.c
OBJ=$(SRC:.c=.o) 

all: $(OBJ)

clean:
	rm -f $(OBJ) *~

.SUFFIXES: .c .o

.c.o:
	$(CC) -c $(CFLAGS) $<; sleep 10
