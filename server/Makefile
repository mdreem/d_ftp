CC=gcc
CFLAGS=-I./include -ansi -g -std=gnu11
LDFLAGS=-lpam -lpam_misc
OBJ_FILES=server_core.o commands.o tools.o auth.o

server: main.o server_core.o commands.o tools.o auth.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

tests: $(OBJ_FILES) test/tools.o
		$(CC) $(CFLAGS) $(LDFLAGS) -lcmocka -o $@ $^

clean:
	rm -rf server_core.o commands.o tools.o auth.o server test/tools.o
