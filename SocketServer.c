#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h> // for close

int main(int argc, char* argv[])
{	
	int port;
	if(argc > 1 )
	{
		port = atoi(argv[1]); //poate e nevoie o conversie
	}
	else
	{
		printf("Please choose a port number (grather than 2000)!");
		exit(0);
	}	


	fd_set readFds,writeFds;
	int socketid, clientSocket;
	struct sockaddr_in server, client;
	socklen_t clientAddressLength;
	char receiveFromClient[100];

	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = htonl(INADDR_ANY);

	//printf("Before socket created!\n");
	if((socketid = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("server: socket() system call");
		exit(0);
	}

	//printf("Before bind()\n");
	if(( bind(socketid, (struct sockaddr*)&server, sizeof(server))) == -1)
	{
		perror("server: bind() system call");
		exit(0);
	}

	//printf("After bind()\n");
	

	if((listen(socketid,50)) < 0)
	{
		perror("server: listen() system call");
		exit(0);
	}	

	printf("Server are waiting for clients...\n");
	
	int maxFd,i;
	while(1)
	{
		FD_ZERO(&readFds);
		FD_ZERO(&writeFds);
		FD_SET(socketid,&readFds);
		maxFd = socketid;

		if(select(maxFd + 1, &readFds, NULL, NULL, NULL) == -1)
		{
			perror("server: select()");
			exit(0);
		}

		for( i=0;i<= maxFd; i++)
		{
			
			if(FD_ISSET(i, &readFds))
			{
				if( i == socketid)
				{	
					clientAddressLength = sizeof(client);
					clientSocket =  accept(socketid, (struct sockaddr*)&client, (socklen_t*)&clientAddressLength );
					if(clientSocket == -1)
					{
						perror("server: accept() system call");
						exit(0);
					}
					//printf("Message accepted!\n");
					else 
					{
						FD_SET(clientSocket, &readFds);
						if(clientSocket > maxFd)
						{
							maxFd = clientSocket;
						}
					}
				}
				else 
				{
					if((recv(clientSocket, &receiveFromClient, sizeof(receiveFromClient),0)) == -1)
					{
						perror("server: recv() system call");
						exit(0);

					}
					printf("Message received!\n");
					printf("The message from the client is: %s\n",receiveFromClient);
					char m[] = "Hello from the server!";
		
					if(send(clientSocket, &m , sizeof(m), 0) == -1)
					{
						perror("server: send() system call");
						exit(0);
					}
					printf("Server send back a message!\n");

				}
			}

			
			
		
		}
	}
	
	if(close(socketid) == -1)
			{
				perror("server: close2() server system call");
				exit(0);
			}
		



	return 0;
}