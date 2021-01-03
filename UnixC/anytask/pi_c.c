// gcc pi_c.c -o pi_c

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <signal.h>

const int PIPE_TO_READ=0;
const int PIPE_TO_WRITE=1;

int main(int argc, char* argv[]) {
	pid_t pid_ac, pid_bc;

	const char * A_bin = "./pi_a";
	const char * B_bin = "./pi_b";

	int pipe_bc[2];
	pipe(pipe_bc);

	int pipe_ab[2];
	pipe(pipe_ab);

	if (argc == 1){
		printf("Set substring to find as first argument.\n");
		exit(1);
	}

	char * substr_to_find = argv[1];

	if ((pid_ac = fork()) == -1){
		perror("fork ac");
		exit(1);
	}

	if (pid_ac > 0)
	{
		close(pipe_ab[PIPE_TO_WRITE]);

		// exec B
		if ((pid_bc = fork()) == -1)
		{
			perror("fork ab");
			exit(1);
		}

		if (pid_bc > 0){
			close(pipe_ab[PIPE_TO_READ]);

			clock_t start = clock();
			// parent - c
			close(pipe_bc[PIPE_TO_WRITE]);
			dup2(pipe_bc[PIPE_TO_READ], 0);
			close(pipe_bc[PIPE_TO_READ]);

			pid_t timer_pid;
			int status = 0;
			if (argc > 2){
				int timeout = atoi(argv[2]);
				if ((timer_pid = fork()) == -1){
					perror("fork timer");
					exit(1);
				}

				if (timer_pid == 0){
					sleep(timeout);
					exit(0);
				}
				else{
					const pid_t finished_first = wait(&status);
					if (finished_first == timer_pid) {
						printf("Timed out.\n");
						kill(pid_ac, SIGHUP);
						kill(pid_bc, SIGHUP);
						waitpid(pid_ac, &status, 0);
						waitpid(pid_bc, &status, 0);
						exit(0);
					} else if (finished_first == pid_ac) {
						kill(timer_pid, SIGKILL);
						waitpid(timer_pid, &status, 0);
					}
				}
			}

			size_t index = -1;
			scanf("%zu", &index);

			clock_t end = clock();
			double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

			printf("'%s' was found. Index: %zu\n", substr_to_find, index);
			printf("Elapsed time: %fs.\n", time_spent);

			kill(pid_ac, SIGHUP);
			waitpid(pid_ac, &status, 0);
			exit(0);
		}
		else{
			dup2(pipe_ab[PIPE_TO_READ], 0);
			close(pipe_ab[PIPE_TO_READ]);

			close(pipe_bc[PIPE_TO_READ]);
			dup2(pipe_bc[PIPE_TO_WRITE], 1);
			close(pipe_bc[PIPE_TO_WRITE]);

			if (execlp(B_bin, B_bin, substr_to_find, NULL) == -1) {
				fprintf(stderr, "Unable to exec B.\n");
				_exit(1);
			}
		}
	}
	else{
		// child  - A
		close(pipe_ab[PIPE_TO_READ]);
		dup2(pipe_ab[PIPE_TO_WRITE], 1);
		close(pipe_ab[PIPE_TO_WRITE]);

		if (execlp(A_bin, A_bin, NULL) == -1) {
			fprintf(stderr, "Unable to exec B.\n");
			_exit(1);
		}
	}
}