CC=gcc
CFLAGS=-I./include -ansi -g -std=gnu1x
LDFLAGS=-lpam -lpam_misc -lpamc
OBJ_FILES=server_core.o commands.o tools.o auth.o

server: main.o server_core.o commands.o tools.o auth.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

tests: $(OBJ_FILES) test/tools.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) -lcmocka

clean:
	rm -rf server_core.o commands.o tools.o auth.o server test/tools.o tests
