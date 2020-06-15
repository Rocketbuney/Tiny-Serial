CC = gcc
CFLAGS = -std=c89 -Wall -pedantic

default: main.o

main.o: src/main.c
	$(CC) $(CFLAGS) src/main.c -o tiny-serial

clean:
	rm tiny-serial
