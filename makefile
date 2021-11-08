all:server client launcher

server: server.c
		gcc -c server.c -o Server

client: client.c
		gcc -c client.c -o Client

launcher: Launcher.c
		gcc `pkg-config --cflags gtk+-3.0` -o launcher Launcher.c `pkg-config --libs gtk+-3.0`
dependency:
	sudo apt install libgtk-3-dev

clean:
	rm -f Server Client launcher
