#include <gtk/gtk.h>
#include <unistd.h>
#define WINDOW_WIDTH 700
#define WINDOW_HEIGHT 500

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#define MAXPORT 80
#define PORT 8080
#define SA struct sockaddr
int sockfdGlobe;
char buffRead[MAXPORT];
int is_Alive = 1;
pthread_mutex_t stdout_lock;
int readCheck = 0;

GtkWidget *buttonServer,*buttonClient; //buttons
GtkWidget *box; //fields
GtkWidget *instructions; //labels
GtkWidget *window; //Application window
GtkWidget *inputField; //input field for user



/*
 * client.c
 *
 *  Created on: Nov 11, 2020
 *      Author: sarwars1
 */
void *read2(void *_args){
	char buff[MAXPORT];
	for(;;){
		readCheck = 0;
		bzero(buff, MAXPORT);
		read(sockfdGlobe, buff, sizeof(buff));
		readCheck = 1;
		fprintf(stdout, "\nFrom other user: %s", buff);
			// if msg contains "Exit" then server exit and chat ended.	
		//pthread_mutex_unlock(&stdout_lock);
		readCheck = 1;
		if (is_Alive == 0) {
			printf("Read thread exit\n");
			break;
		}
	}
	pthread_exit(NULL);
}
void *write2(void *_args){
	
	int n;
	is_Alive = 1;
	for(;;){
		bzero(buffRead, sizeof(buffRead)); 
		fprintf(stdout, "Enter a string: ");
		n = 0;
		bzero(buffRead, MAXPORT);
		//pthread_mutex_lock(&stdout_lock);
		while ((buffRead[n++] = getchar()) != '\n'){
			if(readCheck == 1){
				fprintf(stdout, "Enter a string: %s", buffRead);
			}
		};
		//sleep(2);
		//pthread_mutex_unlock(&stdout_lock);
		write(sockfdGlobe, buffRead, sizeof(buffRead));
		if ((strncmp(buffRead, "exit", 4)) == 0) {
			printf("Client Exit...\n");
			is_Alive = 0;
		}
		// if msg contains "Exit" then server exit and chat ended.
		if (is_Alive == 0) {
			printf("client exit \n");
			break;
		}
	}
	
	pthread_exit(NULL);
}
void func(int sockfd)
{
	if (pthread_mutex_init(&stdout_lock, NULL) != 0) {
		printf("\n mutex init has failed\n");
	}
	pthread_t thread_id1;
	pthread_t thread_id2;
	sockfdGlobe = sockfd;
	pthread_create(&thread_id1, NULL, read2, NULL);
	pthread_create(&thread_id2, NULL, write2, NULL);
	pthread_join(thread_id1, NULL);
	pthread_join(thread_id2, NULL);
}

int chatClient()
{
	int sockfd, connfd;
	struct sockaddr_in servaddr, cli;

	// socket create and varification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);

	// connect the client socket to server socket
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
		printf("connection with the server failed...\n");
		exit(0);
	}
	else
		printf("connected to the server..\n");

	// function for chat
	func(sockfd);

	// close the socket
	close(sockfd);
}




//gui interface code

void entered(GtkEntry *widget, gpointer data){
    
    g_print("you entered: %s\n", gtk_entry_get_text(GTK_ENTRY(data)));
    gchar* IP  = gtk_entry_get_text(GTK_ENTRY(data));
    g_print("IP = %s \n", IP);
    chatClient();
}

void runClient(){
    box = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(window), box);//add box to window

    instructions = gtk_label_new("Please enter the IP of the chat room you wish to join then press ENTER.");//setting the label
    gtk_fixed_put(GTK_FIXED(box), instructions, (WINDOW_WIDTH / 2) -  (72 * 3), 50);

    inputField = gtk_entry_new();//Setup input field
    gtk_entry_set_placeholder_text(GTK_ENTRY(inputField), "x.x.x.x");
    gtk_fixed_put(GTK_FIXED(box), inputField, (WINDOW_WIDTH / 2) - 80, WINDOW_HEIGHT /2);

    g_signal_connect(inputField, "activate", G_CALLBACK(entered), inputField);


    gtk_widget_show_all(window);
}

 static void button_clicked(GtkWidget *widget, char data[]){
    g_print("The %s button was pressed \n", data);

    // logic if server is clicked
    if(data == "server"){
        gtk_container_remove(GTK_CONTAINER(window), box);
    }
    // logic if client is clicked
    if(data == "client"){
        gtk_container_remove(GTK_CONTAINER(window), box);
        runClient();
    }
}

void inizilize (GtkWidget *window){

    instructions = gtk_label_new("What would you like to do?");//setting the label

    //setup box to contain items
    box = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(window), box);//add box to window

    //Add buttons and label to screen
    buttonServer = gtk_button_new_with_label("Host Chat");
    gtk_widget_set_size_request(GTK_WIDGET(buttonServer), 200, 100);
    buttonClient = gtk_button_new_with_label("Join Chat");
    gtk_widget_set_size_request(GTK_WIDGET(buttonClient), 200, 100);
    gtk_fixed_put(GTK_FIXED(box), buttonServer, 100, 250);
    gtk_fixed_put(GTK_FIXED(box), buttonClient, 400, 250);
    gtk_fixed_put(GTK_FIXED(box), instructions, (WINDOW_WIDTH / 2) - 80, 50);

    g_signal_connect(buttonServer, "clicked", G_CALLBACK(button_clicked), "server");
    g_signal_connect(buttonClient, "clicked", G_CALLBACK(button_clicked), "client");
    
}

int main (int argc, char **argv){

    //So this is used to call visual and color maping and initalizes the gtx library.
    gtk_init (&argc, &argv);

    //Displays the window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);// Main window
    
    gtk_window_set_title(GTK_WINDOW(window), "The Awsome Chat Room!");// set title of application
    gtk_window_set_default_size(GTK_WINDOW(window), WINDOW_WIDTH, WINDOW_HEIGHT);// set size in pixles
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);//center window
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);// prevent user from resizing window

    inizilize(window);// pass the window to inizlize

    gtk_widget_show_all(window);

    //close window corectly ending program.
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_main ();// loops the function

    return 0;
}
