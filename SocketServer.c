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
	// poate ceva de la while: accept(): bad file descriptor
	while(1)
	{
		// bad file descriptor, clientSocket se suprascrie
		clientSocket =  accept(socketid, (struct sockaddr*)&client, (socklen_t*)&clientAddressLength );
		if(clientSocket == -1)
		{
			perror("server: accept() system call");
			exit(0);
		}
		printf("Message accepted!\n");

		
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
		}
		printf("Server send back a message!\n");

		
		if(close(clientSocket) == -1)
		{
			perror("server: close() client system call");
			exit(0);
		}
		if(close(socketid) == -1)
		{
			perror("server: close() server system call");
		}
		
	
	}
	/*printf("Waiting for a connection...\n");
	for(;;) {
    int done, n;
    char m[] = "Hello from the server!";
        
        
    if ((clientSocket = accept(socketid, (struct sockaddr *)&client, &clientAddressLength)) == -1) 
    {
        perror("accept");
        exit(1);
    }

    printf("Server accept a client.\n");

    done = 0;
    do 
    {
        n = recv(clientSocket, receiveFromClient, sizeof(receiveFromClient), 0);
        if (n <= 0) 
        {
            if (n < 0) perror("recv");
                done = 1;
        }
        printf("Server received a message!\n");
        printf("The message is: %s\n", receiveFromClient);

        if (!done) 
        {
        	if (send(clientSocket, m, strlen(m), 0) < 0) 
                {
                    perror("send");
                    done = 1;
                	}
                	printf("server send a message back to the client!\n");
            	}

        } while (!done);

        close(clientSocket);
    }*/

	close(clientSocket);
	if(close(socketid) == -1)
	{
		perror("server: close() server system call");
	}
		



	return 0;
}