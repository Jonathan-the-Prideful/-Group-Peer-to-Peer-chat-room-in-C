all:server client

server: server.c
		gcc -c server.c -o Server

client: client.c
		gcc -c client.c -o Client

clean:
	rm -f Server Client
