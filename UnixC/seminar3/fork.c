#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>


void Run(const char* program, const char* arg) {
    pid_t pid = fork();

    if (pid == -1) {
        fprintf(stderr, "Unable to fork\n");
    } else if (pid > 0) {
        int status;
        waitpid(pid, &status, 0);
    } else {
        // we are the child            
        if (execlp(program, program, arg, NULL) == -1) {
            fprintf(stderr, "Unable to exec\n");
            _exit(42);
        }
    }
}
 
int main() {
	long long int mem = 100000000;
	long long int *arr = malloc(mem*sizeof(long long int)); 
	if(arr) {
        for(size_t n=0; n<mem; ++n)
            arr[n] = n;
	}
	
    for (int i = 0; i < 1000; i++) {
        Run("/bin/true", NULL);
    }
	free(arr);
    return 0;
}
