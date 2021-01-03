#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdbool.h>


int main(int argc, char **argv)
{
	if (argc < 3 || argc > 4) {
		printf("Wrong number of args.\n");
		return 1;
	}
	size_t num_bytes = 0;
	if (sscanf (argv[2], "%zu", &num_bytes) != 1) {
		fprintf(stderr, "Please pass valid number of bytes as second argument.");
		return 1;
	}
	bool is_append = argc == 3 ? true : false;
	FILE* fd = is_append
		? open(argv[1], O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR)
	    : open(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	for (size_t i = 0; i < num_bytes; ++i)
	{
		if(is_append)
			lseek(fd, 0, SEEK_END);
		write(fd, "*", 1);		
	}
	close(fd)
	return 0;
}

