CC=		clang -Weverything

DFLAGS=		-DJ64_STATIC

CFLAGS=		-ansi -pedantic -g -O0 \
		-Wno-missing-prototypes \
		-Wno-long-long \
		-Wno-unused-function \
		-Wno-padded

SRC=		j64_test.c

BIN=		j64_test

test: $(SRC)
	$(CC) $(DFLAGS) $(CFLAGS) -o $(BIN) $(SRC)
	./$(BIN)

.PHONY: clean

clean:
	rm -f $(BIN)
