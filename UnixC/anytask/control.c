#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>

long long counter = 0;
FILE* log_f;
char pid_name[8] = "pidfile";

int process_sig_term = 0;

void sig_handler(int sig)
{
	process_sig_term = 1;
}

void handle_sig_term(){
	const char msg[] = "SIGTERM received.\n";
    fprintf(log_f, msg);
	fflush(log_f);
	sleep(1);
	FILE* file = fopen(pid_name, "r");
	int pid = 0;
	fscanf(file, "%d", &pid);  
	fclose(file);
	int del = remove(pid_name);
    if (!del)
      fprintf(log_f, "Pidfile removed successfully.\n");
    else
      fprintf(log_f, "Pidfile removal failed.\n");
    fflush(log_f);
	fclose(log_f);
	kill(pid, SIGKILL);
	process_sig_term = 0;
}

void create_pid_file() {
    FILE* pid_f = fopen("pidfile", "w");
    if (!pid_f) {
        return;
    }
    fprintf(pid_f, "%d\n", (int)getpid());
    fclose(pid_f);
}

int main(int argc, char* argv[]) {
	log_f = fopen("log.txt", "w");
    if (!log_f) {
        fprintf(stderr, "Unable to open log file.\n");
        return 1;
    }
	
    struct sigaction sa;
    sa.sa_handler = sig_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
 
    if (sigaction(SIGTERM, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }
	
	create_pid_file();
	
    while (1) {
		if (process_sig_term){
			handle_sig_term();
		}
        fprintf(log_f, "Counter: %lld\n", counter);
        fflush(log_f);
		++counter;
        sleep(1);
    }
	
    return 0;
}