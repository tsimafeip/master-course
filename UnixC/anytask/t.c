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

const int NUM_THREADS = 4;
const int N = 128;
 
int main(int argc, char** argv) {
	int arr[N];
	int arr1[NUM_THREADS];
	
	int thread_part_size = N / NUM_THREADS;
	
	int cur_thread_num = 0, counter = 0, shift = 0;
	int thread_args[N];
	
	srand(time(NULL));
	for (size_t i = 0; i < N; i++){
		//arr[i] = (rand()%N);
		arr[i] = i;
		counter++;
		if (counter == N-1){
			printf("Thread: %d .", cur_thread_num);
			shift = cur_thread_num*thread_part_size;
			printf("Shift: %d .", shift);
		}
		else{
			if (counter > (cur_thread_num+1)*thread_part_size){
				printf("Thread: %d .", cur_thread_num);
				shift = cur_thread_num*thread_part_size;
				printf("Shift: %d .", shift);
				//memcpy(thread_args[cur_thread_num], arr + shift, sizeof(int) * thread_part_size);
				memcpy(thread_args + shift, arr + shift, sizeof(int) * thread_part_size);
				cur_thread_num+=1;
			}
		}
	}

				printf("\n");
	
	//printf("%d ", cur_start);
	//printf("%d ", cur_end);
	//memcpy(arr1, arr, sizeof(int) * (cur_end));
	
	for (size_t i = 0; i < NUM_THREADS; i++){
		for (size_t j = 0; j < thread_part_size; j++){
			printf("%d \n", thread_args[i+j]);
		}
	}
}