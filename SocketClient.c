#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h> // for close

#define PORT 3011

int main(int argc, char* argv[])
 {
 	//numele fisierului este primit ca parametru
	char numeFisier[10];
	char server_reply[100];
	FILE *fd;
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
	printf("client send the name of the file to the server\n");

	int file_size;
	if(recv(clientid, &server_reply, BUFSIZ, 0) == -1)
	{
		perror("client recv() file_size");
		exit(0);
	}
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
	printf("Client are going to write the data into the file...\n");
	//daca server nu intra in while e posibil ca clientul sa aiba infinte loop
	while( (length = recv(clientid, &server_reply, BUFSIZ,0)) >0 && (remain_data > 0))
	{
		remain_data -= length;
		printf("The client are writing the data\nRemain data:%d", remain_data);

	}

	printf("After the while\n");
	printf("remain_data: %d length: %d\n", remain_data, length);
	if(remain_data == 0 && length >0)
		printf("Client receive the file from the server!\n");
	else
		printf("client: Something went wrong!\n");

	fclose(fd);
	if(close(clientid) == -1)
	{
		perror("client: close() system call");
		exit(0);
	}
	return 0;
 }
	
