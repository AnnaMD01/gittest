#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <mqueue.h>	//for messages queues
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

//#define QUEUE_CLIENT_NAME "/client"
#define QUEUE_SERVER_NAME "/server"
#define SIZE 256
#define MG_MAXMSG 10
#define MG_MSGSIZE 200

int main()
{
	mqd_t client, server;
	char message_from_server[SIZE];
	char queue_client_name[56];
	sprintf(queue_client_name,"/client-%d", getpid());
	//sprintf("%s\n", queue_client_name);
	
	struct mq_attr attr;

	attr.mq_flags = 0;
	attr.mq_maxmsg = MG_MAXMSG;
	attr.mq_msgsize = MG_MSGSIZE;
	attr.mq_curmsgs = 0;
	
	if((client = mq_open(queue_client_name, O_CREAT	| O_RDWR, 0666, &attr)) == -1)
	{
		printf("Client can create /open a messages queue\n");
		exit(0);
	}
		
	if((server = mq_open(QUEUE_SERVER_NAME, O_WRONLY)) == -1)
	{
		printf("Client cann't access the server's messages queue\n");
		exit(0);
	}
		
	if((mq_send(server, queue_client_name, strlen(queue_client_name), 0)) == -1)
	{
		printf("Client cann't send a messages to the server\n");
		exit(0);
	}
		
	if((mq_receive(client, message_from_server, SIZE, NULL)) == -1)
	{
		printf("Client cann't receive a message from the server\n");
		exit(0);
	}
		
		printf("The token number that have just been received from the server is: %s\n",message_from_server);

		//close and erase the message queue
		mq_close(client);
		mq_unlink(queue_client_name);
	
	return 0;
}