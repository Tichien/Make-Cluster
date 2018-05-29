CC = gcc
CFLAGS = -W -Wall -g -std=c99
SRCS1 = prime.c
LIB = -lm
EXE1 = prime
EXT = *.o

$(EXE1): $(SRCS1)
	clear
	$(CC) $(CFLAGS) $(SRCS1) -o $(EXE1) $(LIB)

clean:
	rm -f $(EXT) $(EXE1)
