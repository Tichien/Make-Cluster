CC = gcc
CFLAGS = -W -Wall -g -std=c99
SRCS1 = prime.c
LIB = -lm
EXE1 = bin/prime
EXT = *.o

$(EXE1): $(SRCS1)
	clear
	mkdir -p "bin"
	$(CC) $(CFLAGS) $(SRCS1) -o $(EXE1) $(LIB)

clean:
	rm -f $(EXT)
