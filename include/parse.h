#ifndef PARSE_H
#define PARSE_H

struct dbheader_t
{
	unsigned short version;
	unsigned short count;	
};

int parse_file_header(int fd, int *numEmployeesOut);

#endif
