#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

pthread_mutex_t lock1, lock2;


void *res1(){
	pthread_mutex_lock(&lock1);

	printf("Job started in 1..\n");
	sleep(2);

	printf("Trying to get 2\n");
	pthread_mutex_lock(&lock2); 
	printf("Acquired 2\n");
	pthread_mutex_unlock(&lock2);

	printf("Job finished in 1..\n");

	pthread_mutex_unlock(&lock1);

	pthread_exit(NULL);
}

void *res2(){
	pthread_mutex_lock(&lock2);

	printf("Job started in 2..\n");
	sleep(2);

	printf("Trying to get 1\n");
	pthread_mutex_lock(&lock1); 
	printf("Acquired 1\n");
	pthread_mutex_unlock(&lock1);

	printf("Job finished in 2..\n");

	pthread_mutex_unlock(&lock2);

	pthread_exit(NULL);
}
 
int main(int argc, char** argv) {
	pthread_mutex_init(&lock1,NULL);
	pthread_mutex_init(&lock2,NULL);

	pthread_t t1, t2;

	pthread_create(&t1,NULL,res1,NULL);
	pthread_create(&t2,NULL,res2,NULL);

	pthread_join(t1,NULL);
	pthread_join(t2,NULL);

	return 0;
}