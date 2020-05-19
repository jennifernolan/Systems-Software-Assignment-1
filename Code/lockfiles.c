/* Program changes the permissions for the folder*/
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <syslog.h>
#include <sys/stat.h>

#define LOCK_PATH "/home/jennifer/Documents/Assignment1/var/www/html/live/"

void lock(char *path)
{
	int status;
	struct stat buffer;
	struct stat st;

	mode_t mode;
	stat(path, &st);
	
	//change permissions on file to prevent writing
	mode = st.st_mode & 33188;
	mode &= ~(S_IRUSR);
	mode |= S_IXUSR;

	//modify the permissions of the file to prevent them being editted
	if(chmod(path,mode) < 0)
	{
		openlog("Assignment1",LOG_PID|LOG_CONS,LOG_USER);
		syslog(LOG_INFO, "Lock failed");
		closelog();
	}
	else
	{
		openlog("Assignment1",LOG_PID|LOG_CONS,LOG_USER);
		syslog(LOG_INFO, "Lock succeeded");
		closelog();
	}
	status = stat(path, &buffer);
}

void unlock(char *path)
{

	char mode[] = "0777";
	int i = 0;

	i = strtol(mode, 0, 8);

	//change the permissons of the file back so that the file can be editted again
	if(chmod(path,i) < 0)
	{
		openlog("Assignment1",LOG_PID|LOG_CONS,LOG_USER);
		syslog(LOG_INFO, "Unlock failed");
		closelog();
	}
	else
	{
		openlog("Assignment1",LOG_PID|LOG_CONS,LOG_USER);
		syslog(LOG_INFO, "Unlock succeeded");
		closelog();
	}
}
