#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>


int main()
{  
	char file[8] = "a.txt";
	FILE* fd1 = open(file, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	FILE* fd2 = dup(fd1);
	FILE* fd3 = open(file, O_RDWR);
	write(fd1, "Hello,", 6);
        write(fd2, "world", 6);
	lseek(fd2, 0, SEEK_SET);
	write(fd1, "HELLO,", 6);
	write(fd3, "Gidday", 6);
	return 0;
}

