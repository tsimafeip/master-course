#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
 
bool GetFilesize(const char* filename, size_t* size) {
    struct stat st;
    if (stat(filename, &st) == -1) {
        perror("stat");
        return false;
    }
    *size = st.st_size;
    return true;
}
 
int main(int argc, char** argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s FILE_SRC FILE_TRG\n", argv[0]);
        return 2;
    }
 
    size_t filesize;
    if (!GetFilesize(argv[1], &filesize)) {
        return 1;
    }
	
    //Open file
    int fd_src = open(argv[1], O_RDWR);
    if (fd_src == -1) {
        perror("open src");
        return 1;
    }
 
    // Execute mmap
    char* mmappedData_src = mmap(0, filesize, PROT_READ, MAP_PRIVATE | MAP_POPULATE, fd_src, 0);
    if (mmappedData_src == MAP_FAILED) {
        perror("mmap");
        return 1;
    }
	
	int fd_trg = open(argv[2], O_CREAT | O_RDWR, S_IRWXU | S_IRWXG | S_IRWXO);
    if (fd_trg == -1) {
        perror("open trg");
        return 1;
    }
	
	int tr_res = ftruncate(fd_trg, filesize);
	if (tr_res == -1) {
        perror("truncate failed");
        return 1;
    }
	
	char* mmappedData_trg = mmap(NULL, filesize, PROT_WRITE | PROT_READ, MAP_PRIVATE, fd_trg, 0);

    if (mmappedData_trg == MAP_FAILED) {
        perror("mmap trg");
        return 1;
    }
	
	for (size_t i = 0; i < filesize; i++){
		mmappedData_trg[i] = mmappedData_src[filesize-1-i];
	}
	
	dup2(fd_trg, 1);
	printf("%s", mmappedData_trg);
	
	//Cleanup
    int rc = munmap(mmappedData_src, filesize);
    if (rc != 0) {
        perror("munmap src");
    }
	
	int rc2 = munmap(mmappedData_trg, filesize);
    if (rc2 != 0) {
        perror("munmap trg");
    }
	
    close(fd_src);
	close(fd_trg);
}