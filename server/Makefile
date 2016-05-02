CC=gcc
CFLAGS=-I./include -ansi -g -std=gnu1x
LDFLAGS=-lpam -lpam_misc

server: server_core.o commands.o tools.o auth.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

clean:
	rm -rf server_core.o commands.o tools.o auth.o server
