/* server file controls input from client and allows for on demand backup and update depending of the users input*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <unistd.h>

#include "lockfiles.h"
#include "backup.h"
#include "update.h"

#define INTRANET_PATH "/home/jennifer/Documents/Assignment1/var/www/html/intranet/index.html"
#define LIVE_PATH "/home/jennifer/Documents/Assignment1/var/www/html/live/index.html"
#define BACKUP_PATH "/home/jennifer/Documents/Assignment1/var/www/html/backup/"

int main()
{
	char live_path[255];
	char backup_path[255];
	char intranet_path[255];

	strcpy(live_path,LIVE_PATH);
	strcpy(backup_path,BACKUP_PATH);
	strcpy(intranet_path,INTRANET_PATH);

	mqd_t mq;
	struct mq_attr queue_attributes;
	char buffer[1024 + 1];
	int terminate = 0;
	
	//setup the queue
	queue_attributes.mq_flags = 0;
	queue_attributes.mq_maxmsg = 10;
	queue_attributes.mq_msgsize = 1024;
	queue_attributes.mq_curmsgs = 0;

	//open the queue with the specified name
	mq = mq_open("/Queue", O_CREAT | O_RDONLY, 0644, &queue_attributes);

	do {
		//listen for input from the queue
		ssize_t bytes_read;
		bytes_read = mq_receive(mq, buffer, 1024, NULL);
		buffer[bytes_read] = '\0';
		//if the user enters exit terminate the queue
		if(! strncmp(buffer, "exit", strlen("exit")))
		{
			terminate = 1;
		}
		//if the user enters backup, backup the live site
		else if (! strncmp(buffer, "backup", strlen("backup")))
		{
			lock(live_path);
			sleep(5);
			backupDocs(live_path, backup_path);
			unlock(live_path);
		}
		//if the user enters update, update the live site with the contents
		//of the intranet site
		else if (! strncmp(buffer, "update", strlen("update")))
		{
			lock(live_path);
			sleep(5);
			updateSite(intranet_path, live_path);
			unlock(live_path);
		}
		else
		{
			printf("Please enter \"backup\" to backup the live site or enter \"update\" to update the live site : %s", buffer);
		}
	} while(!terminate);

	mq_close(mq);
	mq_unlink("/Queue");
	return 0;
}
