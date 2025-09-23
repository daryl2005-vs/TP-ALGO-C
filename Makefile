CC=gcc
CFLAGS=-Iinclude -Wall -Wextra -std=c11
SRC=src/main.c
OBJ=$(SRC:.c=.o)
BIN=bin/main

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f src/*.o $(BIN)

.PHONY: all clean
