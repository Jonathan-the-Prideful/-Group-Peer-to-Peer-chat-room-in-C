/*
 * server.c
 *
 *  Created on: Nov 11, 2020
 *      Author: sarwars1
 */
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#define MAX 80
#define PORT 8080
#define SA struct sockaddr
#define CLOCKS_PER_SC 100000
/*void recieve(int sockid, int sockid2){
	char buff[MAX];
	bzero(buff, MAX);
	read(sockid, buff, sizeof(buff));
	write(sockfd2, buff, MAX);
}
// Function designed for chat between client and server.*/
int actionTimerCounter = 0;

int actionTimer(){
	
	int msec = 0,trigger =10; //10ms wait time.
	
	clock_t before = clock();
	
	do{
		//something
		clock_t difference =clock() - before;
		msec = difference * 1000 /CLOCKS_PER_SC;
		//iterations++;
	}while( msec < trigger );
	actionTimerCounter ++;
	return 0;	
}
void chatRoom(int sockfd, int sockfd2)
{
	
	char buff[MAX];
	int n;
	// infinite loop for chat
	for (;;) {
		bzero(buff, MAX);

		// read the message from client and copy it in buffer
		//read
		read(sockfd, buff, sizeof(buff));//first user
		// print buffer which contains the client contents
		actionTimer();
		//write
		write(sockfd2, buff, sizeof(buff));//send
		
		bzero(buff, MAX);
		
		read(sockfd2, buff, sizeof(buff)); //second user
		actionTimer();
		write(sockfd, buff, sizeof(buff));

		// if msg contains "Exit" then server exit and chat ended.
		if (strncmp("exit", buff, 4) == 0) {
			printf("Server Exit...\n");
			break;
		}
	}
}

// Driver function
int main()
{
	
	int sockfd, sockfd2, connfd, len;
	int userCount = 0;
	int userArray[2];
	struct sockaddr_in servaddr, cli;
	

	// socket create and verification
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
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);
	
	
	// Binding newly created socket to given IP and verification
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
		printf("socket bind failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully binded..\n");
	
	while(userCount < 2){
	// Now server is ready to listen and verification
	if ((listen(sockfd, 5)) != 0) {
		printf("Listen failed...\n");
		exit(0);
	}
	else
		printf("Server listening..\n");
	len = sizeof(cli);

	// Accept the data packet from client and verification
	userArray[userCount] = accept(sockfd, (SA*)&cli, &len);
	if (connfd < 0) {
		printf("server acccept failed...\n");
		exit(0);
	}
	else
		printf("server acccept the client...\n");
		userCount ++;
	}
	
	// Function for chatting between client and server
	chatRoom(userArray[0], userArray[1]);

	// After chatting close the socket
	close(sockfd);
	printf("\n*** Number of times action timer ran: %d***", actionTimerCounter);
	
	
}

