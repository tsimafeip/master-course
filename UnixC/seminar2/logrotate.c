#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>


volatile sig_atomic_t sighuped = 0;


void sighup_handler(int sig)
{   
    sighuped = 1;
}

long long counter = 0;

void CreatePidFile() {
    FILE* f = fopen("pidfile", "w");
    if (!f) {
        return;
    }
    fprintf(f, "%d\n", (int)getpid());
    fclose(f);
}
 
void PrintLine(FILE* f) {
    time_t ltime;
    struct tm result;
    char stime[32];
 
    ltime = time(NULL);
    localtime_r(&ltime, &result);
    strftime(stime, sizeof(stime), "%F %T", &result);
    fprintf(f, "[%s] PID: %d, Counter: %lld\n", stime, (int)getpid(), counter);
    ++counter;
}

int RunMain() {
    CreatePidFile();

	signal(SIGHUP, sighup_handler);
    
    FILE* log_f = fopen("log.txt", "a");
    if (!log_f) {
        fprintf(stderr, "unable to open file\n");
        return 1;
    }

    while (1) {
        if (sighuped){
            fclose(log_f);
            log_f = fopen("log.txt", "w");
            sighuped = 0;
        }
        PrintLine(log_f);
        fflush(log_f);
        usleep(10000);
    }
    fclose(log_f);
}

int main(int argc, char* argv[]) {
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    }
    if (pid == 0) {
        return RunMain();
    } else {
        fprintf(stderr, "Started a daemon with PID = %d.\n", pid);
        return 0;
    }
    return 0;
}