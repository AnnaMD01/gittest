#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main(int argc, char* argv[])
{	
	int port;
	if(argc > 1 )
	{
		port = atoi(argv[1]); //poate e nevoie o conversie
	}
	else
	{
		printf("Please choose a port number!");
		exit(0);
	}	

	int socketid, clientSocket;
	struct sockaddr_in server, client;
	unsigned int clientAddressLength = 0;
	char receiveFromClient[100];

	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = htonl(INADDR_ANY);


	if((socketid = socket(PF_UNIX, SOCK_STREAM, 0)) == -1)
	{
		perror("server: socket() system call");
		exit(0);
	}

	if(( bind(socketid, (struct sockaddr*)&server, sizeof(server))) == -1)
	{
		perror("server: bind() system call");
		exit(0);
	}

	if((listen(socketid,10)) == -1)
	{
		perror("server: listen() system call");
		exit(0);
	}	


	while(1)
	{	
		
		if((clientSocket = accept(socketid, (struct sockaddr*)&client, &clientAddressLength)) == -1)
		{
			perror("server: accept() system call");
			exit(0);
		}

		// numele fisierului solicitat de catre client 
		// este trimis ca cerere
		// trebuie sa trimita un fisier sau "File not found"
		// incerc cu un mesaj

		//receive message from client
		if(recv(clientSocket, receiveFromClient, sizeof(receiveFromClient),0) == -1)
		{
			perror("server: recv() system call");
			exit(0);

		}
		
		if(send(clientSocket,"Message", sizeof("Message"),0) == -1)
		{
			perror("server: send() system call");
		}

		if(close(clientSocket) == -1)
		{
			perror("server: close() system call");
			exit(0);
		}
	}	
	



	return 0;
}