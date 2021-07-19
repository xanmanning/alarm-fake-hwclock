#include <stdio.h>
#include <time.h>
#include <utime.h>
#include <sys/stat.h>
#include <sys/time.h>

/* If the difference between the last known time and current time
 * is greater than this amount, we assume the clock is broken
 * and reset it. */
static time_t max_diff = 60 * 60 * 24 * 365 * 10; /* ten years */

int main (int argc, char *argv[])
{
	struct stat temp;
	const char *timefile;
	/* The time is stored in the creation and modification date of a file.
	 * If no specific file is passed, use the executable itself.
	 */
	if (argc > 1)
		timefile = argv[1];
	else
		timefile = argv[0];
	time_t current_time;
	time_t time_store;
	time_t diff;

	current_time = time(NULL);
	if(current_time == ((time_t) - 1))
		return 1;

	if(stat(timefile, &temp) < 0)
		return 1;

	time_store = temp.st_mtime;
	diff = time_store - current_time;

	if(time_store > current_time || diff > max_diff || diff < -max_diff)
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
