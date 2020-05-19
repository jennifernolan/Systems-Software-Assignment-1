/* Program that backs up the site files to the backup folder */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <syslog.h>
#include <sys/wait.h>

#include "timestamp.h"

void backupDocs(char *src, char *dest)
{	
	//get the current date to append to backed up file
	char date[50];
	char *time = date_toString(date);
	strcat(dest,time);

	//copy the live site file to the backup folder
	char *arg[] = {"cp", "-r", src, dest, NULL};

	//execute the copy command
	if(!vfork())
	{
		execv("/bin/cp", arg);
	}
	wait(NULL); //to prevent a zombie process

	//log the backup to the system
	openlog("Assignment1",LOG_PID|LOG_CONS,LOG_USER);
	syslog(LOG_INFO, "Backup succeeded");
	closelog();
}
