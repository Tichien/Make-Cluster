CC = gcc
CFLAGS = -W -Wall -g
SRCS1 = main.c
LIB = 
EXE1 = cluster
HEADER = includes
EXT = *.o

$(EXE1): $(SRCS1)
	clear
	$(CC) $(CFLAGS) $(SRCS1) -o $(EXE1) $(LIB)

clean:
	rm -f $(EXT)
