//makefile
// Created on: Nov 4, 2021
//     Author: Jonathan
all: Launcher server client

Launcher: Launcher.o
		gcc Launcher.o -o Launcher

Launcher.o: Launcher.c
		gcc -c Launcher.c -o Launcher.o

server: server.o
		gcc server.o -o server

server.o: server.c
		gcc -c server.c -o server.o

client: client.o
		gcc client.o -o client

client.o: client.c
		gcc -c client.c -o client.o

clean:
	rm -f Launcher.o server.o client.o