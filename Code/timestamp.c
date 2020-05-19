/* Program to convert timestamp, i.e. date, to string - needed for backup.c*/

#include <stdio.h>
#include <time.h>

char *date_toString(char *date)
{
	time_t now;
	struct tm *currDate;

	time(&now); //get the current time
	currDate = localtime(&now);

 	//change from time to string
	strftime(date, 80, "%d %m %Y", currDate);
	
	return date;
}
