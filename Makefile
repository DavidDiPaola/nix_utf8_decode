#2018 David DiPaola
#licensed under CC0 (public domain, see https://creativecommons.org/publicdomain/zero/1.0/)

BIN = utf8_decode

.PHONY: all
all: $(BIN)

$(BIN): main.c
	$(CC) -Wall -Wextra -O2 $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -rf $(BIN)

