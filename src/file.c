#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include "file.h"

int open_file_rw(char *filename)
{
	int fd = open(filename, O_RDWR);	
	if (fd == -1)
	{
		perror("open");
		return -1;
	}
	return 0;
}

int open_file_r(char *filename)
{
	int fd = open(filename, O_RDONLY);	
	if (fd == -1)
	{
		perror("open");
		return -1;
	}
	return 0;
}

int open_file_w(char *filename)
{
	int fd = open(filename, O_WRONLY);
	if (fd == -1)
	{
		perror("open");
		return -1;
	}
	return 0;
}
