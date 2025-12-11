#include <stdio.h>
#include <unistd.h>


#include "parse.h"

int parse_file_header(int fd, int *numEmployeesOut)
{
	if (fd == -1)
	{
		printf("Bad file descriptor provided.\n");
		return -1;
	}
	else
	{
		printf("File descriptor accepted");
	}
	
	struct dbheader_t header = {0};
	if (read(fd, &header, sizeof(header) != sizeof(header)))
	{
		printf("Failed to read file header");
		return -1;
	}

	*numEmployeesOut = header.count;
	return 0;

}
