#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include "./src/get_next_line.h"

int main()
{
	int fd;
	char *line = NULL;

	fd = open("test.txt", O_RDONLY);
	if (fd == -1)
	{
		perror("Error opening file");
		return 1;
	}
	while ((line = get_next_line(fd)) != NULL)
	{
		printf("printing: %s\n", line);
		free(line);
		line = NULL;
	}
	if (line != NULL)
		free(line);
	if (close(fd) == -1)
	{
		perror("Error closing file");
		return 1;
	}
	return 0;
}