#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "user/threads.h"

#define NUM_THREADS 4

// Do nothing, strictly for testing purposes
void *start_fn(void *arg) {
  char *tid = *(char **)arg;

  printf("Thread %s: Hello from start!\n", tid);
  
  sleep(2);

  printf("Thread %s exiting!\n", tid);

  return 0;
}

int main(int argc, char **argv) {
  thread_t threads[NUM_THREADS];
  char *tid[9];
  int retvals[9];


  // Come back to this
  for (int i = 0; i < NUM_THREADS; i++) {
    char str[2];
  	char c = 'a' + i;
  	str[0] = c;
  	str[1] = 0;
  	tid[i] = str;
  }

  // printf("Test for xv6\n");
  // printf("\n--------= Test for thread_create API call =--------\n\n");

  for (int i = 0; i < 9; i++) {
    if (thread_create(&threads[i], &start_fn, &tid[i]) == -1) {
      break;
    }
	printf("Thread ID for %s given by API: %d\n", tid[i], threads[i]);
  }

  // printf("\n--------= Test for thread_join API call =--------\n\n");
 
   for (int i = 0; i < 4; i++) {
     thread_join(&threads[i], (void **)(&retvals[i]));
   }
  
  // printf("\n--------= Test for thread_cancel API call =--------\n\n");

  // thread_cancel(&threads[2]);

  exit(0);
}
