#include <stdio.h>
#include <time.h>
#include <utime.h>
#include <sys/stat.h>

int main (int argc, char *argv[]) 
{
	struct stat temp;
	struct utimbuf time_update;
	const char *timefile = argv[0];
	char date_command[100];
	char raw_date_command[50] = "date --set=";
	time_t current_time;
	time_t time_store;

	current_time = time(NULL);
	if(current_time == ((time_t) - 1))
		return 1;

	if(stat(timefile, &temp) < 0)
		return 1;

	time_store = temp.st_mtime;

	if(time_store > current_time) 
	{
		time_t timebuffer = time_store;
		printf("Setting Clock.");
		sprintf(date_command, "%s'%s'", raw_date_command, ctime(&timebuffer));
		system(date_command);
	} else {
		time_update.actime = temp.st_atime;
		time_update.modtime = time(NULL);
		printf("Saving current time.");
		utime(timefile, &time_update);
	}

	return 0;
}
