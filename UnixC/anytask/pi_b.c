// gcc pi_b.c -o pi_b

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <signal.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/types.h>

volatile sig_atomic_t kill_b = 0;

void sighup_b_handler(int sig) {
	kill_b = 1;
}

void runB(const char const *s) {
	int len_of_substr = strlen(s);
	size_t index = 0;
	
	char * buffer_to_read = calloc(2*len_of_substr + 1, sizeof(char));
	
	for (int i = 0; i < 2*len_of_substr; i++){
		scanf("%c", buffer_to_read+i);
	}	
	
	while (!kill_b) {
		char *result = strstr(buffer_to_read, s);
		
		if (result != NULL) {
			int position = result - buffer_to_read;
			printf("%zu", index + position);
			break;
		} else {
			index += len_of_substr;
			
			for (int i = 0; i < 2*len_of_substr; i++){
				if (i < len_of_substr)
					*(buffer_to_read+i) = *(buffer_to_read+i+len_of_substr); // copy second part to the start
				else
					scanf("%c", buffer_to_read+i); // read new dara
			}
		}
	}
	
	free(buffer_to_read);
}

int main(int argc, char* argv[]) {
	if (argc != 2){
		printf("Wrong number of args.");
		exit(1);
	}

	struct sigaction sa_b;
	sa_b.sa_handler = sighup_b_handler;
	sa_b.sa_flags = 0;
	sigemptyset(&sa_b.sa_mask);

	if (sigaction(SIGHUP, &sa_b, NULL) == -1) {
		perror("sigaction for B");
		exit(1);
	}

	char *s = argv[1];
	runB(s);

	return 0;
}