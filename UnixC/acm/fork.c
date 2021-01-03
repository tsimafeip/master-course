#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
  int status;
  int child_pid = fork();
  if (child_pid > 0){
    printf("[parent] My PID is %d\n", getpid());
	printf("[parent] Child's PID is %d\n", child_pid);
	waitpid(child_pid, &status, 0);
	printf("[parent] Child exited with code %d\n", WEXITSTATUS(status));
  }
  else{
	  printf("[child] Hello, my PID is %d\n", getpid());
	  sleep(1);
	  printf("[child] Bye\n");
	  exit(42);
  }
  return 0;
}