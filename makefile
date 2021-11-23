all:server client launcher

server: server.c
		gcc  server.c -o Server -lpthread 

client: client.c
		gcc  client.c -o Client -lpthread

launcher: Launcher.c
		gcc Launcher.c -o launcher -lpthread
		
dependency:
	sudo apt install libgtk-3-dev

clean:
	rm -f Server Client launcher
