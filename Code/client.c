/* program to allow the user to do on demand backups and updates using a message queue*/
#include <stdio.h>
#include <string.h>
#include <mqueue.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	mqd_t mq;
	char buffer[1024];

	//open the queue that will be written to
	mq = mq_open("/Queue", O_WRONLY);

	printf("Please enter \"backup\" to backup the live site or enter \"update\" to update the live site\n");

	//allow the user to enter inputs
	do {
		printf(">> ");
		fflush(stdout);
		memset(buffer, 0, 1024);
		fgets(buffer, 1024, stdin);
		mq_send(mq, buffer, 1024, 0);
	}while (strncmp(buffer, "exit", strlen("exit"))); //if the user inputs exit 
	//exit the client and server

	mq_close(mq);
	return 0;
}
