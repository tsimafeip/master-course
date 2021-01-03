#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

#define MSGSIZE 1

int main (int argc, char *argv[]){	
	int pipefd[2];
	pid_t childpid;
	
	char str[] = "a";
	
	if (pipe(pipefd) == -1) {
		fprintf(stderr, "Unable to create pipe.\n");
		return 1;
	}
	
	if ((childpid = fork()) == -1)
    {
        perror("fork");
        exit(1);
    }

    if (childpid == 0)
    {
        close(pipefd[0]);
	
		int cnt = 0;
		// while(1){
			write(pipefd[1], str, (strlen(str)));
			// cnt++;
			// printf("%d ", cnt);
		// }
        exit(0);
    }
    else
    {
        close(pipefd[1]);
		while(1){
		}
    }
	
	return 0;
}