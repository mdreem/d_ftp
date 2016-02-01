server: server_core.o commands.o tools.o
	gcc -o server server_core.o commands.o tools.o

server_core.o: server_core.c
	gcc -g3 -c server_core.c

commands.o: commands.c
	gcc -g3 -c commands.c

tools.o: tools.c
	gcc -g3 -c tools.c

clean:
	rm -rf server_core.o commands.o tools.o server
