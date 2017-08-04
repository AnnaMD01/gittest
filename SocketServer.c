#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h> // for close
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sendfile.h>

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
					printf("The client request for the file: %s\n",receiveFromClient);
					char f1[] = "file1";
					char f2[] = "file2";
					char f3[] = "file3";
					char f4[] = "file4";
					char f5[] = "file5";

					if( (strcmp (receiveFromClient, f1) == 0) || (strcmp (receiveFromClient, f2) == 0) || (strcmp (receiveFromClient, f3) == 0) || (strcmp (receiveFromClient, f4) == 0) || (strcmp (receiveFromClient, f5) == 0))
					{
						printf("The file is available!\nThe client request the file: %s\n",receiveFromClient);
						struct stat file_stat;

						int fd = open(receiveFromClient,O_RDONLY);
						if ( fd < 0)
						{
							perror("open()");
							exit(0);
						}
						if (fstat(fd, &file_stat)<0)
						{
							perror("fstat");
							exit(0);
						}
						char file_size[256];
						sprintf(file_size, "%ld", file_stat.st_size);
						printf("The file size is: %s\n", file_size);
						if(send(clientSocket, file_size, sizeof(file_size),0) == -1)
						{
							perror("server send() the size of the file");
							exit(0);

						}
						printf("Server send the size of the file:%s\n",file_size);
						int offset = 0;
						int remain_data = file_stat.st_size;
						int sb = 0;
						//printf("offset: %d remain_data: %d sb: %d\n", offset, remain_data,sb);
						printf("Server are going to send the data...\n");
						// server nu intra in while
						// pentru ca sb = 0;
						sb = sendfile(clientSocket,fd, (off_t*)&offset, BUFSIZ);
						printf("sb: %d\n", sb);
						//remain_data -=sb;
						//printf("remain_data: %d\n", remain_data);
						while( ((sb = sendfile(clientSocket,fd, (off_t*)&offset, BUFSIZ)) > 0) && (remain_data > 0))
						{
							remain_data -=sb;
							printf("Server are sending the data...\nRemain data:%d", remain_data);
						}
						//printf("After the while\n");
						//printf("offset: %d remain_data: %d sb: %d\n", offset, remain_data,sb);
						if(sb < 0)
							printf("Somwthing went wrong! The file was not transmited!");
						if(remain_data == 0) 
							printf("Server send the file %s succesfully!", receiveFromClient);
						

					}
					else 
					{
						if(send(clientSocket, "FILE NOT FOUND", sizeof("FILE NOT FOUND"),0) == -1)
						{
							perror("server send() FILE NOT FOUND");
							exit(0);
						}

					}

					

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