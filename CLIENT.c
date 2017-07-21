#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <mqueue.h>	//for messages queues
#include <fcntl.h>
#include <sys/types.h>

#define MESSAGE_QUEUE_CLIENT_NAME "client"
#define MESSAGE_QUEUE_SERVER_NAME "server"
#define SIZE 256

int main()
{
	mqd_t client, server;
	char message_from_client[56];
	sprintf (message_from_client, "process id is %d", getpid ());
	char message_from_server[SIZE];

	if((client = mq_open(MESSAGE_QUEUE_CLIENT_NAME, O_CREAT	| O_RDWR, 0666, NULL)) != -1)
	{	
		printf("Just for testing!\n");
		server = mq_open(MESSAGE_QUEUE_SERVER_NAME, O_WRONLY);
		mq_send(server, message_from_client, strlen(message_from_client), 0);
		mq_receive(client, message_from_server, SIZE, NULL);

		printf("The token number that have just been received from the server is: %s\n",message_from_server);

		//close and erase the message queue
		mq_close(client);
		mq_unlink(message_from_client);
	}


	return 0;
}