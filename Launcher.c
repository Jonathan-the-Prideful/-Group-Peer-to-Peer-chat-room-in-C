#define WINDOW_WIDTH 700
#define WINDOW_HEIGHT 500

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#define MAXPORT 80
#define PORT 8080
#define SA struct sockaddr
#define defualtIP "127.0.0.1"
int sockfdGlobe;
char buffRead[MAXPORT] = {'a','b','c'};
int is_Alive = 1;

/*
 * client.c
 *
 *  Created on: Nov 11, 2020
 *      Author: ALeonard, JThomas
 */
void *read2(void *_args){
	char buff[MAXPORT];
	for(;;){
		readCheck = 0;
		bzero(buff, MAXPORT);
		read(sockfdGlobe, buff, sizeof(buff));
		readCheck = 1;
		fprintf(stdout, "\nFrom other user: %s \nEnter a string:", buff);
			// if msg contains "Exit" then server exit and chat ended.	
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
		while ((buffRead[n++] = getchar()) != '\n'){
		};
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
	setvbuf(stdout, NULL, _IONBF, 0);
	pthread_t thread_id1;
	pthread_t thread_id2;
	sockfdGlobe = sockfd;
	sleep(10);
	pthread_create(&thread_id1, NULL, read2, NULL);
	pthread_create(&thread_id2, NULL, write2, NULL);
	pthread_join(thread_id1, NULL);
	pthread_join(thread_id2, NULL);

}

int chatClient(char IP[])
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
	//servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_addr.s_addr = inet_addr(IP);
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

int main (int argc, char **argv){
	char hostOrClient[4];
	printf("Would you like to \"Host\" or \"Join\"?\n"); 
	scanf("%s", hostOrClient);
	printf("%s", hostOrClient);
	// logic if server is clicked
    if(strcmp(hostOrClient, "Host") || strcmp(hostOrClient, "host")){
		system("./Server &");	
		sleep(2);//Need time for the sever to fire up 
		chatClient(defualtIP);
    }
    // logic if client is clicked
    if(strcmp(hostOrClient, "Join") || strcmp(hostOrClient, "join")){
		char ip[16];
		printf("\nPlease enter the IP of the chat room you wish to join then press ENTER.\n");
		scanf("%s",ip);
		chatClient(ip);
    }
	printf("we ended");
    return 0;
}
