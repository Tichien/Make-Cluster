CC = g++
CFLAGS = -W -Wall -g
SRCS1 = prime.cpp
LIB = 
EXE1 = bin/prime
EXT = *.o

$(EXE1): $(SRCS1)
	clear
	mkdir -p "bin"
	$(CC) $(CFLAGS) $(SRCS1) -o $(EXE1) $(LIB)

clean:
	rm -f $(EXT)
