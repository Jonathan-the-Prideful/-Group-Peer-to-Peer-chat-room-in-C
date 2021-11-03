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
char buffRead[MAX];
int is_Alive = 1;
pthread_mutex_t stdout_lock;
int readCheck = 0;
void *read2(void *_args){
	char buff[MAX];
	for(;;){
		readCheck = 0;
		bzero(buff, MAX);
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
		bzero(buffRead, MAX);
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
	if (pthread_mutex_init(&stdout_lock, NULL) != 0) {
		printf("\n mutex init has failed\n");
	}
	pthread_t thread_id1;
	pthread_t thread_id2;
	sockfdGlobe = sockfd;
	pthread_create(&thread_id1, NULL, read2, NULL);
	pthread_create(&thread_id2, NULL, write2, NULL);
	/*char buff[MAX];
	int n;
	for(;;){
		bzero(buff, sizeof(buff)); 
		printf("Enter a string: ");
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
	}*/
	pthread_join(thread_id1, NULL);
	pthread_join(thread_id2, NULL);
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

