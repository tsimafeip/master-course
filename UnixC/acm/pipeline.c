#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <stdbool.h> 

size_t overall_number_of_commands=0;
const int PIPE_TO_READ=0;
const int PIPE_TO_WRITE =1;

void set_args(char *cur_args[], char *source_args[], size_t end, size_t number){
	size_t start = end-number;
	for(size_t i = 0; i < number; i++){
		cur_args[i]=source_args[start+i];
	}
	
	cur_args[number] = NULL;
}

int update_commands(int* command_ends, int * command_lengths, const int end, const int number_of_args){
	if (number_of_args == 0){
		fprintf(stderr, "Incorrect order of separators.\n");
		return 2;
	}
	
	*(command_ends + overall_number_of_commands) = end;
	*(command_lengths + overall_number_of_commands) = number_of_args;

	++overall_number_of_commands;
	return 0;
}

int main (int argc, char *argv[]){	
	if(argc == 1){
		fprintf(stderr, "Arguments list is empty.\n");
		return 2;
	}

	int command_ends[argc];
	int command_lengths[argc];
	size_t number_of_args = 0;
	int result_of_run;

	for (int i=1; i<argc; i++)
    {
		if (*argv[i] == '|' && *(argv[i]+1) == '\0'){
			result_of_run = update_commands(command_ends, command_lengths, i, number_of_args);
			if(result_of_run !=0)
				return result_of_run;
			number_of_args = 0;
		}
		else{
			number_of_args++;
		}
    }
	
	result_of_run = update_commands(command_ends, command_lengths, argc, number_of_args);
	if (result_of_run != 0)
		return result_of_run;	
	
	//fprintf(stdout, "C> Created %i pipes.\n", overall_number_of_commands-1);
	
	pid_t pid_arr[overall_number_of_commands + 1];
	int pipefds[overall_number_of_commands][2];
	
	for (int i = 0; i < overall_number_of_commands; i++) {
		if (pipe(pipefds[i]) == -1) {
			fprintf(stderr, "Unable to create %i-pipe.\n", i);
			if (i > 0) {
                close(pipefds[i - 1][PIPE_TO_READ]);
            }
			return 1;
		}
		
		if ((pid_arr[i] = fork()) == 0) {
			if (i > 0){
				close(0);
				if (dup(pipefds[i-1][PIPE_TO_READ]) == -1) {
					fprintf(stderr, "Dup failed on copying read pipe. PID: %d. I:%i\n", getpid(), i);
					_exit(1);
				}
				close(pipefds[i-1][PIPE_TO_READ]);
			}
			
			if (i < (overall_number_of_commands-1)){
				close(pipefds[i][PIPE_TO_READ]);
				close(1);
				if (dup(pipefds[i][PIPE_TO_WRITE]) == -1) {
					fprintf(stderr, "Dup failed on copying write pipe. PID: %d. I:%i\n", getpid(), i);
					_exit(1);
				}
				close(pipefds[i][PIPE_TO_WRITE]);
			}
			
			//child code
			int number_of_command = i;
			size_t number_of_args = command_lengths[number_of_command];
			char* local_args[number_of_args+1];
			set_args(local_args, argv, command_ends[number_of_command], number_of_args);
			if (execvp(local_args[0], local_args) == -1) {
				fprintf(stderr, "Unable to exec.\n");
				_exit(1);
			}
		}
		else{
			if(pid_arr[i] == -1){
				fprintf(stderr, "Failed to fork.\n");
				if (i > 0) {
					close(pipefds[i - 1][PIPE_TO_READ]);
				}
				close(pipefds[i][PIPE_TO_READ]);
				close(pipefds[i][PIPE_TO_WRITE]);
				return 1;
			}
			else{
				close(pipefds[i][1]);
				if (i > 0) {
					close(pipefds[i - 1][0]);
				}
			}
		}
	}
	
	for (int i=0; i < overall_number_of_commands; i++) {
	    int status = 0;
		waitpid(pid_arr[i], &status, 0);
		if (WEXITSTATUS(status) || WIFSIGNALED(status)) 
		{
			return 1;
		}
	}
	
	return 0;
}