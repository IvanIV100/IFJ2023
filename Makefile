CC = gcc
CFLAGS =  -Wall -Wextra -Wpedantic -g -lm
SRC = main.c parser.c    symtable.c parametrs.c  error.c expressions.c  code_generator.c scanner.c
OBJ = $(SRC:.c=.o)
EXEC = main

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ) $(EXEC)