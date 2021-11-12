all:server client launcher

server: server.c
		gcc  server.c -o Server -lpthread 

client: client.c
		gcc  client.c -o Client -lpthread

launcher: Launcher.c
		gcc `pkg-config --cflags gtk+-3.0` -o launcher Launcher.c `pkg-config --libs gtk+-3.0`
		
dependency:
	sudo apt install libgtk-3-dev

clean:
	rm -f Server Client launcher
