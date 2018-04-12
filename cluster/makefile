CC = gcc
CFLAGS = -W -Wall -g
SRCS1 = cluster.c
LIB = 
EXE1 = bin/cluster
HEADER = includes
EXT = *.o

$(EXE1): $(SRCS1)
	clear
	mkdir -p "bin"
	$(CC) $(CFLAGS) $(SRCS1) -o $(EXE1) $(LIB)

clean:
	rm -f $(EXT)
