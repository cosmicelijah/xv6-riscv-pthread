#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "user/threads.h"

static int NUM_THREADS = 1;

// Do nothing, strictly for testing purposes
void *start_fn(void *arg) {
  char *tid = *(char **)arg;

  printf("Thread %s: Hello from start!\n", tid);
  
  sleep(2);

  printf("Thread %s exiting!\n", tid);

  return 0;
}

int main(int argc, char **argv) {
  if (argc == 1) {
  	NUM_THREADS = 1;
  } else {
  	NUM_THREADS = atoi(argv[1]);
  }

  // --------= Initialize necessary thread data =--------

  thread_t threads[NUM_THREADS];
  char *tid[NUM_THREADS];
  int retvals[9];

  for (int i = 0; i < NUM_THREADS; i++) {
    char *str = malloc(sizeof(char) * 2);
  	char c = 'a' + i;
  	str[0] = c;
    str[1] = 0;
    tid[i] = str;
  }

  // --------= Test for thread_create API call =--------

  for (int i = 0; i < NUM_THREADS; i++) {
    if (thread_create(&threads[i], &start_fn, &tid[i]) == -1) {
      printf("Out of thread slots!\n");
      break;
    }
	printf("Thread ID for %s given by API: %d\n", tid[i], threads[i]);
  }

  // --------= Test for thread_join API call =--------
 
   for (int i = 0; i < NUM_THREADS; i++) {
     thread_join(&threads[i], (void **)(&retvals[i]));
   }
  
  // printf("\n--------= Test for thread_cancel API call =--------\n\n");

  // thread_cancel(&threads[2]);

  exit(0);
}
