#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 2017

int main(int argc, char* argv[])
 {
 	//numele fisierului este primit ca parametru
	char numeFisier[20];
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

	if((clientid = socket(PF_UNIX, SOCK_STREAM, 0)) == -1)
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
	if((send( clientid, numeFisier, strlen(numeFisier),0)) == -1)
	{
		perror("client: send() ssytem call");
		exit(0);
	}

	if( recv(clientid, server_reply, strlen(server_reply),0 ) == -1)
	{
		perror("client: recv() system call");
		exit(0);
	}

	if(close(clientid) == -1)
	{
		perror("client: close() system call");
		exit(0);
	}
	return 0;
 }
	
