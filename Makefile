CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -g -lm
SRC = parser.c  symtable.c scanner.c parametrs.c error.c
OBJ = $(SRC:.c=.o)
EXEC = ifj23

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ) $(EXEC)