/*
 * client.c
 *
 *  Created on: Nov 11, 2020
 *      Author: sarwars1
 */
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#define MAX 80
#define PORT 8080
#define SA struct sockaddr
int sockfdGlobe;

struct thread_args {
	int sockfd;
	int is_Alive;
	};
void *read2(void *_args){
	char buff[MAX];
	struct thread_args *args = (struct thread_args *) _args;
	int *sockfd = args->sockfd;
	for(;;){
		bzero(buff, MAX);
		read(sockfdGlobe, buff, sizeof(buff));
		//if(strncmp(buff, "", 0) != 0){
			fprintf(stdout, "\nFrom other user: %s", buff);
			// if msg contains "Exit" then server exit and chat ended.	
		//}
		if (args->is_Alive == 0) {
			printf("Read thread exit\n");
			break;
		}
	}
	pthread_exit(NULL);
}
/*void *write(void *_args){
	
	char buff[MAX];
	struct thread_args *args = (struct thread_args *) _args;
	for(;;){
		bzero(buff, MAX);
		while ((buff[n++] = getchar()) != '\n');
		write(sockfd, buff, sizeof(buff));
		if ((strncmp(buff, "exit", 4)) == 0) {
			printf("Client Exit...\n");
			args->is_Alive = 0;
		}
		// if msg contains "Exit" then server exit and chat ended.
		if (args->is_Alive == 0) {
			printf("Read thread exit\n");
			break;
		}
	}
	
	pthread_exit(NULL);
}*/
void func(int sockfd)
{
	/*char buff[MAX];
	int n;
	for (;;) {
		bzero(buff, sizeof(buff));
		printf("Enter the string : ");
		n = 0;
		while ((buff[n++] = getchar()) != '\n');
		write(sockfd, buff, sizeof(buff));
		bzero(buff, sizeof(buff));
		read(sockfd, buff, sizeof(buff));
		printf("From Server : %s", buff);
		if ((strncmp(buff, "exit", 4)) == 0) {
			printf("Client Exit...\n");
			break;
		}
	}*/
	pthread_t thread_id1;
	struct thread_args trash;
	struct thread_args *args = malloc (sizeof(trash));
	args -> sockfd = sockfd;
	args -> is_Alive = 1;
	sockfdGlobe = sockfd;
	pthread_create(&thread_id1, NULL, read2, &args);
	char buff[MAX];
	int n;
	for(;;){
		bzero(buff, sizeof(buff));
		printf("Enter the string : ");
		n = 0;
		bzero(buff, MAX);
		while ((buff[n++] = getchar()) != '\n');
		//sleep(2);
		write(sockfd, buff, sizeof(buff));
		if ((strncmp(buff, "exit", 4)) == 0) {
			printf("Client Exit...\n");
			args->is_Alive = 0;
		}
		// if msg contains "Exit" then server exit and chat ended.
		if (args->is_Alive == 0) {
			printf("client exit exit\n");
			break;
		}
	}
}

int main()
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

