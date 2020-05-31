#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int interval = 5;	// time between two frames change to 5 after debugging
int totalh = 1;		// total number of frame to collect
int fps = 5;

int main(int argc, char *argv[])
{
	if(argc >= 4)
		fps = atoi(argv[3]);	// fps
	if(argc >= 3)
		interval = atoi(argv[2]);// take interval
	if(argc >=2)
	{
		if(argv[1][0] == '-')
		{
			if(argv[1][1] == 'h')
			{
				printf("camlog [total hour] [take interval] [fps]\n");
				return 0;
			}
		}
		else
			totalh = atof(argv[1]);	// total hour ///// problem on this? test atof
	}
	
	//initialize
	int total = totalh * 3600 / interval;
	time_t ttt = time(NULL);
	struct tm *tt = localtime(&ttt);
	char cmd[127];		// cache for fswebcam command
	char t_string[32];	// cache for printing time
	char path[32];		// path for saving files
	sprintf(t_string, "%02d%02d%02d_%02d%02d%02d", tt->tm_year, tt->tm_mon, tt->tm_mday, tt->tm_hour, tt->tm_min, tt->tm_sec);
	sprintf(path, "zkv_%s", t_string);
	sprintf(cmd, "mkdir %s", path);
	system(cmd);
	

	//start recording
	for(int i = 0; i < total; i++)
	{
		sprintf(cmd, "fswebcam -q -r 1024x768 --no-banner %s/record_%03d.jpg", path, i+1);
		system(cmd);
		printf("current: %03d, %03d left\n", i+1, total - i);
		sleep(interval);
	}

	//generate video
	printf("Converting...\n");
	//sprintf(cmd, "ffmpeg -y -v 0 -i %s/record_%%03d.jpg -filter:v \"setpts=%0.1f*PTS\" zkvid_%s.mp4", path, cratio / 25.0 / interval, t_string);
	sprintf(cmd, "ffmpeg -v 0 -r %d -i %s/record_%%3d.jpg zkvid_%s.mp4", fps, path, t_string);
	system(cmd);
	printf("Done.\n");
	return 0;
}
