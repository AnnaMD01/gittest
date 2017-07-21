#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>	//for messages queues
#include <fcntl.h>
#include <sys/types.h>

#define MESSAGE_QUEUE_SERVER_NAME	"server"
#define SIZE 	256

int main()
{
	mqd_t server,client;	//messages queue identifier
	long tokenNumber = 0;
	char buffer_receive_server[SIZE];
	char buffer_send_to_client[SIZE];

	
	
	if((server = mq_open(MESSAGE_QUEUE_SERVER_NAME,	O_CREAT	| O_RDWR, 0666, NULL)) != -1);
		{
			while(1) // a server should always listen
			{	
				mq_receive(server, buffer_receive_server, SIZE, NULL);
				client = mq_open(buffer_receive_server, O_WRONLY);
				sprintf(buffer_send_to_client,"%ld",tokenNumber);
				mq_send(client, buffer_send_to_client, strlen(buffer_send_to_client), 0);
				//mq_close(client);
				tokenNumber++;
			}
		}
	

	return 0;

}