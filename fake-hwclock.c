#include <stdio.h>
#include <time.h>
#include <utime.h>
#include <sys/stat.h>
#include <sys/time.h>

int main (int argc, char *argv[]) 
{
	struct stat temp;
	const char *timefile = argv[0];
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
		struct timeval sys_update;
		sys_update.tv_sec =time_store;
		sys_update.tv_usec=0;
		printf("Setting Clock.\n");
		settimeofday(&sys_update,NULL);
	} else {
		struct utimbuf time_update;
		time_update.actime = temp.st_atime;
		time_update.modtime = time(NULL);
		printf("Saving current time.\n");
		utime(timefile, &time_update);
	}

	return 0;
}
