CC = gcc
CFLAGS = -std=gnu99 -Wall -Wextra -Wpedantic -g -lm
SRC = main.c parser.c   expressions.c  symtable.c parametrs.c  error.c scanner.c
OBJ = $(SRC:.c=.o)
EXEC = main

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ) $(EXEC)