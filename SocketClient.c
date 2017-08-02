#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 3007

int main(int argc, char* argv[])
 {
 	//numele fisierului este primit ca parametru
	char numeFisier[10];
	char server_reply[100];
	if(argc > 1)
	{
		strcpy(numeFisier, argv[1]);
		//printf("Numele fisierului trimis ca parametru este: %s\n",numeFisier);
	}
	else 
	{
		printf("Please send a file name!");
		exit(0);
	}

	int clientid;

	if((clientid = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("client: socket() system call");
		exit(0);
	}

	// connect to the server
	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	if(connect(clientid, (struct sockaddr*)&server, sizeof(server)) == -1)
	{
		perror("client: connect() system call");
		exit(0);
	}
	printf("Client is connecting to the server\n");
	if((send( clientid, &numeFisier, sizeof(numeFisier),0)) == -1)
	{
		perror("client: send() ssytem call");
		exit(0);
	}
	printf("client send a message to the server\n");
	if( recv(clientid, &server_reply, sizeof(server_reply),0 ) == -1)
	{
		perror("client: recv() system call");
		exit(0);
	}
	server_reply[100] = '\0';
	printf("client received a message from the server\n");
	printf("The message is: %s\n", server_reply);

	if(close(clientid) == -1)
	{
		perror("client: close() system call");
		exit(0);
	}
	return 0;
 }
	
