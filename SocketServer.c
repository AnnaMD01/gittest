#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h> 
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sendfile.h>

int main(int argc, char* argv[])
{	
	int port;
	if(argc > 1 )
	{
		port = atoi(argv[1]); 
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

	
	if((socketid = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("server: socket() system call");
		exit(0);
	}

	
	if(( bind(socketid, (struct sockaddr*)&server, sizeof(server))) == -1)
	{
		perror("server: bind() system call");
		exit(0);
	}

	
	

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
						

						
						FILE* fd1 = fopen(receiveFromClient,"r");
						
						if(fd1 == NULL)
						{
							perror("Open the file send by client!\n");
							exit(0);
						}
						
						fseek(fd1, 0, SEEK_END);
						long fsize = ftell(fd1);
						char b[256];
						sprintf(b,"%ld",fsize);
						
						rewind(fd1);
						
						if(send(clientSocket, (const void*)b, sizeof(b),0) == -1)
						{
							perror("server send() the size of the file");
							exit(0);
						
						}
						char buffer[fsize];
						int result = fread( buffer, fsize, 1, fd1);
						printf("buffer:%s\n", buffer);
						
						fclose(fd1);
						int remain_data = fsize;
						int sb = 0;
						
						
						while((sb = send(clientSocket, buffer, sizeof(buffer), 0) > 0 ) && (remain_data > 0))
						{
							remain_data -=sb;
							printf("Server are sending the data...\nRemain data:%d\n", remain_data);
						}
						
								
						

						printf("after the while\nremain_data: %d sb: %d\n",  remain_data, sb);
						
						if(sb <= 0 || remain_data > 0)
							printf("Somwthing went wrong! The file was not transmited!");
						else 
							{
								printf("Server send the file: %s succesfully!\n", receiveFromClient);
								printf("Server attends another client!\n");
							}
						

					}
					else 
					{
						if(send(clientSocket, "FILE NOT FOUND", sizeof("FILE NOT FOUND"),0) == -1)
						{
							perror("server send() FILE NOT FOUND");
							exit(0);
						}
						printf("The client demand a FILE that i have NOT FOUND!\n");
						printf("Server attends another file name or another client!\n");

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