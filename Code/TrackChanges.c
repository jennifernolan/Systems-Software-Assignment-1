/* Program to track the changes to the intranet file */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <syslog.h>

void trackChanges()
{
	//command to gather all logs made about file watch on intranet site
	char *args = "ausearch -f /home/jennifer/Documents/Assignment1/var/www/html/intranet/ > /home/jennifer/Documents/Assignment1/SiteModificationLog.txt";

	//execute the command and return to system logs whether it was successful or not
	if(system(args) < 0)
	{
		openlog("Assignment1",LOG_PID|LOG_CONS,LOG_USER);
		syslog(LOG_INFO, "Audit search unsuccessful");
		closelog();
	}
	else
	{
		openlog("Assignment1",LOG_PID|LOG_CONS,LOG_USER);
		syslog(LOG_INFO, "Audit search successful");
		closelog();
	}
	
}
