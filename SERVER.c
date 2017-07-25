#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>	//for messages queues
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define QUEUE_SERVER_NAME	"/server"
#define SIZE 	256
#define MG_MAXMSG 10
#define MG_MSGSIZE 200


int main()
{
	mqd_t server,client;	//messages queue identifier
	long int tokenNumber = 0;
	char receive_from_client[SIZE];
	char send_to_client[SIZE];

	struct mq_attr attr;

	attr.mq_flags = 0;
	attr.mq_maxmsg = MG_MAXMSG;
	attr.mq_msgsize = MG_MSGSIZE;
	attr.mq_curmsgs = 0;

	printf("Server is waiting for messages...\n");

	if((server = mq_open(QUEUE_SERVER_NAME,	O_CREAT	| O_RDWR, 0666, &attr)) == -1)
	{	
		printf("Server is not able to create / open a messages queue!\n");
		exit(0);
	}
			
	while(1) // a server should always listen
	{	
				
		//memset(receive_from_client,0,SIZE);
		//memset(send_to_client,0,SIZE);

		if((mq_receive(server, receive_from_client, SIZE, NULL)) == -1)
		{	perror("Server receive");
			printf("Server can receive messages!\n");
			exit(0);
		}

		if((client = mq_open(receive_from_client, O_WRONLY)) == -1)
		{
			printf("The server can access the client message queue\n");
			exit(0);
		}

		sprintf(send_to_client,"%ld",tokenNumber);
		if((mq_send(client, send_to_client, strlen(send_to_client), 0)) == -1)
		{
			printf("Sorry...the server can send a response to the client\n");
			exit(0);
		}

		tokenNumber++;
	}
	
	return 0;

}