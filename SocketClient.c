#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h> 

#define PORT 3015

int main(int argc, char* argv[])
 {
 	
	char numeFisier[10];
	char server_reply[14];
	
	FILE *fd;
	if(argc > 1)
	{
		strcpy(numeFisier, argv[1]);
		
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
	if((send(clientid, numeFisier, sizeof(numeFisier), 0)) == -1)
	{
		perror("client: send() ssytem call");
		exit(0);
	}
	printf("client send the name of the file to the server\n");

	int file_size;
	if(recv(clientid, server_reply, sizeof(server_reply), 0) == -1)
	{
		perror("client recv() file_size");
		exit(0);
	}

	if(strcmp(server_reply,"FILE NOT FOUND") == 0)
	{
		//sa pun o eticheta goto
		printf("FILE NOT FOUND!\n");
	}
	else
	{
		file_size = atoi(server_reply);
		printf("Client receive the size of the file: %d\n", file_size);
		fd = fopen("ClientFile","w");
		if( fd == NULL)
		{
			perror("client fopen");
			exit(0);
		}
		printf("Client open ClientFile\n");

		int remain_data = file_size;
		int length = 0;
		printf("remain_data: %d length: %d\n", remain_data, length);
		printf("Client is going to write the data into the file...\n");
		
		char buffer[file_size];
		while( ((length = recv(clientid, buffer, sizeof(buffer), 0)) > 0) && (remain_data > 0))
		{
			printf("buffer: %s", buffer);
			fwrite(buffer, sizeof(char),length, fd);
			remain_data -= length;
			printf("The client is writing the data\nRemain data:%d\n", remain_data);

		}
		fclose(fd);

		printf("After the while\n");
		printf("remain_data: %d length: %d\n", remain_data, length);
		if(remain_data == 0 && length == file_size)
			printf("Client receive the file from the server!\n");
		else
			printf("client: Something went wrong!\n");

		}

	
	if(close(clientid) == -1)
	{
		perror("client: close() system call");
		exit(0);
	}
	printf("The end of the program!\n");
	return 0;
 }
	
