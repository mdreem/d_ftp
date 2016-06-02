CC=gcc
CFLAGS=-I./include -ansi -g -std=gnu1x
LDFLAGS=-lpam -lpam_misc -lpamc
OBJ_FILES=server_core.o commands.o tools.o auth.o

server: main.o server_core.o commands.o tools.o auth.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

test_tools: $(OBJ_FILES) test/tools.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) -lcmocka

test_auth: $(OBJ_FILES) test/auth.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) -lcmocka -Wl,--wrap=pam_start,--wrap=pam_authenticate,--wrap=pam_end

tests: test_tools test_auth

clean:
	rm -rf server_core.o commands.o tools.o auth.o server test/tools.o test_tools test_auth
