/* Program that updates the intranet site files to the live site */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <syslog.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>

void updateSite(char *src, char *dest)
{	
	time_t now;
	struct tm* currTime;
	char buffer[26];

	time(&now);
	currTime = localtime(&now);

	char *log = "Live site updated on ";

	//create the command that will copy the intranet site contents to the live site
	char *arg[] = {"cp", "-R", src, dest, NULL};

	//create the update log file
	FILE *fptr;
	fptr = fopen("/home/jennifer/Documents/Assignment1/UpdateLog.txt", "a+");

	//execute the above command
	if(!vfork())
	{
		execv("/bin/cp", arg);
	}
	wait(NULL); //used to prevent zombie processes

	//log the update in the log file with the time and date of the update
	fprintf(fptr, "%s", log);
	fprintf(fptr, "%s\n\n", asctime(currTime));
	fclose(fptr);

	openlog("Assignment1",LOG_PID|LOG_CONS,LOG_USER);
	syslog(LOG_INFO, "Update succeeded");
	closelog();
}
