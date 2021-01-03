#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX 1000

struct TContext {
    const char* Name;
    int* Counter;
    int Mod;
};

pthread_mutex_t mutex;
pthread_cond_t condition;

void custom_wait_or_signal(int thread_mod){
	// we are always sending signal from odd and wait in the even
	if (thread_mod)
		pthread_cond_signal(&condition);
	else
		pthread_cond_wait(&condition, &mutex);	
}

void* ThreadFunc(void* arg) {
  struct TContext *ctxt = arg;
  pthread_mutex_lock(&mutex);
  int *counter = ctxt->Counter;
  pthread_mutex_unlock(&mutex);
  fprintf(stderr, "This is %s thread\n", ctxt->Name);
  int print_any = 1;
  
  while (print_any) {
	  pthread_mutex_lock(&mutex);
	  
	  if (*counter % 2 == ctxt->Mod) {
		  printf("%d ", (*counter)++);
		  print_any = *counter + 1 < MAX;
	  }
	  
	  custom_wait_or_signal(ctxt->Mod);
	  pthread_mutex_unlock(&mutex);
  }
  
  pthread_exit(0);
}

int main()
{
    pthread_t t1;
    pthread_t t2;

    int counter = 0;
    struct TContext ctxt1 = {"even", &counter, 0};
    struct TContext ctxt2 = {"odd", &counter, 1};
    pthread_create(&t1, 0, ThreadFunc, &ctxt1);
    pthread_create(&t2, 0, ThreadFunc, &ctxt2);

    pthread_join(t1, 0);
    pthread_join(t2, 0);
	
    printf("\n");
    return 0;
}