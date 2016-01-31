receive: server_core.o commands.o
	gcc -o receive server_core.o commands.o

server_core.o: server_core.c
	gcc -g3 -c server_core.c

commands.o: commands.c
	gcc -g3 -c commands.c

clean:
	rm -rf server_core.o commands.o receive

