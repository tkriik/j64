CC=		clang
CFLAGS=		-std=c99 -pedantic -Wall -Wextra -g -O0

SRC=		j64.c j64_test.c
BIN=		j64_test

test: $(SRC)
	$(CC) $(CFLAGS) -o $(BIN) $(SRC)
	./$(BIN)

.PHONY: clean

clean:
	rm -f $(OBJ)
