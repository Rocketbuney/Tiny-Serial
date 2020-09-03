CC = gcc
BIN = tiny-serial
CFLAGS = -std=c89 -Wall -pedantic

default: tiny-serial.c
	$(CC) $(CFLAGS) tiny-serial.c -o $(BIN)

install:
	mv $(BIN) /usr/local/bin/

uninstall:
	rm -rf /usr/local/bin/$(BIN)

clean:
	rm $(BIN)
