#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <string.h>

# define NUM_THREADS 1
# define N 1048576

const int NUMS_PER_THREAD = N / NUM_THREADS;
int arr[N];

int cmp(const void *a,const void *b) {
	int *x = (int *) a;
	int *y = (int *) b;
	return *x - *y;
}

void *thread_merge_sort(void* arg)
{
    int thread_id = (*((int*) arg)) - 1;
	
	printf("In thread: thread %d has completed\n", thread_id);
	qsort (arr + thread_id * NUMS_PER_THREAD, NUMS_PER_THREAD, sizeof(int), cmp);

    return NULL;
}

void merge(int left, int middle, int right) {
    int left_length = middle - left + 1;
    int right_length = right - middle;
    int left_array[left_length];
    int right_array[right_length];

    for (size_t i = 0; i < left_length; i ++) {
        left_array[i] = arr[left + i];
    }
    
    for (size_t j = 0; j < right_length; j ++) {
        right_array[j] = arr[middle + 1 + j];
    }
    
	size_t i = 0, j = 0, k = 0;
    while (i < left_length && j < right_length) {
        if (left_array[i] <= right_array[j]) {
            arr[left + k] = left_array[i];
            i ++;
        } else {
            arr[left + k] = right_array[j];
            j ++;
        }
        k ++;
    }
	
    while (i < left_length) {
        arr[left + k] = left_array[i];
        k ++;
        i ++;
    }
    while (j < right_length) {
        arr[left + k] = right_array[j];
        k ++;
        j ++;
    }
}

void merge_array(int number, int aggregation) {
    for(size_t i = 0; i < number; i = i + 2) {
        int left = i * (NUMS_PER_THREAD * aggregation);
        int right = ((i + 2) * NUMS_PER_THREAD * aggregation) - 1;
        int middle = left + (NUMS_PER_THREAD * aggregation) - 1;
        if (right >= N) {
            right = N - 1;
        }
		
        merge(left, middle, right);
    }
	
    if (number / 2 >= 1) {
        merge_array(number / 2, aggregation * 2);
    }
}

int main(int argc, char** argv) {
	srand(time(NULL));
	for (size_t i = 0; i < N; i++){
		arr[i] = (rand()%N);
	}
	
	pthread_t threads[NUM_THREADS];
	int result_code;
	
    for (size_t i = 0; i < NUM_THREADS; i++) {
        result_code = pthread_create(&threads[i], NULL, thread_merge_sort, &i);
		assert(!result_code);
    }
    
    for (size_t i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    merge_array(NUM_THREADS, 1);
	
	return 0;
}