/* This program creates a daemon to continually manage the operation of the requirements
listed by the CTO. The web content is backed up and updated every night*/

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <syslog.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/inotify.h>
#include <syslog.h>

#include "lockfiles.h"
#include "backup.h"
#include "update.h"
#include "TrackChanges.h"

#define INTRANET_PATH "/home/jennifer/Documents/Assignment1/var/www/html/intranet/index.html"
#define LIVE_PATH "/home/jennifer/Documents/Assignment1/var/www/html/live/index.html"
#define BACKUP_PATH "/home/jennifer/Documents/Assignment1/var/www/html/backup/"
 
int main()
{	
	time_t now;
	struct tm newday;
	double seconds;

	time(&now); //get the current time
	newday = *localtime(&now);
	
	//set the hours, mins and secs for the time to update (i.e. night time)
	newday.tm_hour = 23;
	newday.tm_min = 59;
	newday.tm_sec = 00;

	//place a watch on the intranet file for any edits made by users
	char *arg = "auditctl -w /home/jennifer/Documents/Assignment1/var/www/html/intranet/ -p rwxa";
	if(system(arg) > 0)
	{
		openlog("Assignment1",LOG_PID|LOG_CONS,LOG_USER);
		syslog(LOG_INFO, "File watch unsuccessful");
		closelog();
	}
	else
	{
		openlog("Assignment1",LOG_PID|LOG_CONS,LOG_USER);
		syslog(LOG_INFO, "File watch successful");
		closelog();
	}

	// Create a child process      
    	int pid = fork();
 
    	if (pid > 0) {
        	// if pid > 0 it is the parent
        	printf("Parent process\n");
        	sleep(10);
        	exit(EXIT_FAILURE);
    	} else if (pid == 0) {
       		// Create the orphan process
       		printf("Child process\n");
       
       		// Elevate the orphan process to session leader to loose controlling TTY
       		if (setsid() < 0) { exit(EXIT_FAILURE); }

       		// Fork here again to guarantee that the process is not a session leader
       		int pid = fork();
       		if (pid > 0) {
          		exit(EXIT_FAILURE);
       		} else if(pid ==0) {
       			printf("Child child process\n");
          		// Call umask() to set the file mode creation mask to 0
          		// This allows the daemon to read and write files with the permissions required 
          		umask(0);

          		// Change the current working dir to root.
          		// This will eliminate any issues of running on a mounted drive, 
          		// that could be removed
          		if (chdir("/") < 0 ) { exit(EXIT_FAILURE); }

          		// Close all open file descriptors
          		int x;
          		for (x = sysconf(_SC_OPEN_MAX); x>=0; x--)
          		{
            			close(x);
          		}

          		// Signal Handler goes here

          		// Log file goes here

          		// Orphan Logic goes here!! 
          		// Keep process running with infinite loop
          		// When the parent finishes

          		while(1) {
				sleep(1);
				time(&now);
				seconds = difftime(now, mktime(&newday));
				printf("Seconds: %.2f\n", seconds);

				if(seconds == 0)
				{
					char live_path[255];
					char backup_path[255];
					char intranet_path[255];
					strcpy(live_path,LIVE_PATH);
					strcpy(backup_path,BACKUP_PATH);
					strcpy(intranet_path,INTRANET_PATH);

					//Backup initiated here
					lock(live_path);
					sleep(5); // used to demonstrate locking
					backupDocs(live_path, backup_path);
					trackChanges();
					updateSite(intranet_path, live_path);
					unlock(live_path);
				}
          		}
       		}
    	}
    	return 0;
}
