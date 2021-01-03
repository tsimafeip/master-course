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
volatile sig_atomic_t kill_a = 0, kill_b = 0;
//A
void sighup_a_handler(int sig) {
    kill_a = 1;
}
mpz_t tmp1, tmp2, t5, t239, pows;
void actan(mpz_t res, unsigned long base, mpz_t pows)
{
	int i, neg = 1;
	mpz_tdiv_q_ui(res, pows, base);
	mpz_set(tmp1, res);
	for (i = 3; ; i += 2) {
		mpz_tdiv_q_ui(tmp1, tmp1, base * base);
		mpz_tdiv_q_ui(tmp2, tmp1, i);
		if (mpz_cmp_ui(tmp2, 0) == 0) break;
		if (neg) mpz_sub(res, res, tmp2);
		else	  mpz_add(res, res, tmp2);
		neg = !neg;
	}
}
char * get_digits(int n, size_t* len)
{
	mpz_ui_pow_ui(pows, 10, n + 20);
	actan(t5, 5, pows);
	mpz_mul_ui(t5, t5, 16);
	actan(t239, 239, pows);
	mpz_mul_ui(t239, t239, 4);
	mpz_sub(t5, t5, t239);
	mpz_ui_pow_ui(pows, 10, 20);
	mpz_tdiv_q(t5, t5, pows);
	*len = mpz_sizeinbase(t5, 10);
	return mpz_get_str(0, 0, t5);
}
int runA(){
	unsigned long accu = 16384, done = 0;
	size_t got;
	char *s;
	mpz_init(tmp1);
	mpz_init(tmp2);
	mpz_init(t5);
	mpz_init(t239);
	mpz_init(pows);
	while (!kill_a) {
		s = get_digits(accu, &got);
		/* write out digits up to the last one not preceding a 0 or 9*/
		got -= 2; /* -2: length estimate may be longer than actual */
		while (s[got] == '0' || s[got] == '9') got--;
		printf("%d\n", (int)(got - done));
		fflush(stdout);
		printf("%.*s\n", (int)(got - done), s + done);
		fflush(stdout);
		free(s);
		done = got;
		/* double the desired digits; slows down at least cubically */
		accu *= 2;
	}
	return 0;
}
//end A
//B
void sighup_b_handler(int sig) {
    kill_b = 1;
}
int runB(const char *s) {
	size_t index = 0;
	while (!kill_b) {
		int size;
		scanf("%d", &size);
		fflush(stdout);
		char * cur_string = malloc(size);
		scanf("%s", cur_string);
		char *result = strstr(cur_string, s);
		if (result != NULL) {
			size_t position = result - cur_string;
			printf("%zu\n", index + position);
			break;
		} else {
			index += size;
		}
		free(cur_string);
	}
	return 0;
}
//end B
int main(int argc, char* argv[]) {
	int pipe_bc[2];
    pipe(pipe_bc);
    pid_t pid_ac;
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
		clock_t start = clock();
		// parent - c
		close(pipe_bc[PIPE_TO_WRITE]);
		dup2(pipe_bc[PIPE_TO_READ], 0);
		close(pipe_bc[PIPE_TO_READ]);
		pid_t timer_pid;
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
				int status = 0;
				const pid_t finished_first = waitpid(WAIT_ANY, &status, 0);
				if (finished_first == timer_pid) {
					printf("Timed out.\n");
					kill(pid_ac, SIGHUP);
					exit(0);
				} else if (finished_first == pid_ac) {
					kill(timer_pid, SIGKILL);
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
		exit(0);
	}
	else{
		// child  - A
		int pipe_ab[2];
		pipe(pipe_ab);
		pid_t pid_ab;
		if ((pid_ab = fork()) == -1)
		{
			perror("fork ab");
			exit(1);
		}
		if (pid_ab == 0){
			// child - B
			close(pipe_ab[PIPE_TO_WRITE]);
			dup2(pipe_ab[PIPE_TO_READ], 0);
			close(pipe_ab[PIPE_TO_READ]);
			close(pipe_bc[PIPE_TO_READ]);
			dup2(pipe_bc[PIPE_TO_WRITE], 1);
			close(pipe_bc[PIPE_TO_WRITE]);
			struct sigaction sa_b;
			sa_b.sa_handler = sighup_b_handler;
			sa_b.sa_flags = 0;
			sigemptyset(&sa_b.sa_mask);
			if (sigaction(SIGHUP, &sa_b, NULL) == -1) {
				perror("sigaction for B");
				exit(1);
			}
			runB(substr_to_find);
			exit(0);
		}
		else{
			// parent - a
			close(pipe_ab[PIPE_TO_READ]);
			dup2(pipe_ab[PIPE_TO_WRITE], 1);
			close(pipe_ab[PIPE_TO_WRITE]);
			struct sigaction sa_a;
			sa_a.sa_handler = sighup_a_handler;
			sa_a.sa_flags = 0;
			sigemptyset(&sa_a.sa_mask);
			if (sigaction(SIGHUP, &sa_a, NULL) == -1) {
				perror("sigaction for A");
				exit(1);
			}
			runA();
			kill(pid_ab, SIGHUP);
			exit(0);
		}
	}
}