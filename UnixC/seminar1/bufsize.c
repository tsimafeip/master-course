#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdbool.h>


void WriteToStream(FILE* stream, size_t num_bytes){
			
	for (size_t i = 0; i < num_bytes; ++i)
	{
		fprintf(stream, "%s", "*");		
	}
}

int main(int argc, char **argv)
{
	if (argc != 3 && argc != 4) {
		printf("%i\n", argc);
		printf("Wrong number of args.\n");
		return 1;
	}
	
	size_t num_bytes = 0;
	if (sscanf (argv[1], "%zu", &num_bytes) != 1) {
		fprintf(stderr, "Please pass valid number of bytes as second argument.");
		return 1;
	}
	
	FILE* fd;
	switch (argv[2][0])
	{
		case '1':
		  fd = fopen(argv[3], "w+");
		  WriteToStream(fd, num_bytes);
		  fclose(fd);
		  break;
		case '2':
		  WriteToStream(stdout, num_bytes);
		  break;
	    case '3':
		  WriteToStream(stderr, num_bytes);
		  break;
		default:
		  	fprintf(stderr, "Invalid mode. 1 for file, 2 stdout, 3 stderr.");
			return 1;
	}

	return 0;
}