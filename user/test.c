#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "user/threads.h"

// Do nothing, strictly for testing purposes
void *start_fn(void *arg) {
  char *tid = *(char **)arg;

  printf("Thread %s: Hello from start!\n", tid);

  // for (int i = 0; i < 100; i++) {
  // 	printf("Thread %s: testing thread interleaving %d\n", tid, i);
  // }

  printf("Thread %s exiting!\n", tid);

  return 0;
}

int main(int argc, char **argv) {
  thread_t threads[4] = {0, 1, 2, 3}; 
  char *tid[4] = {"a", "b", "c", "d"};
  // int retvals[4];

  // printf("Test for xv6\n");
  // printf("\n--------= Test for thread_create API call =--------\n\n");

  for (int i = 0; i < 4; i++) {
	thread_create(&threads[i], &start_fn, &tid[i]);
  }

  // printf("\n--------= Test for thread_join API call =--------\n\n");

//   int retval;
// 
//   for (int i = 0; i < 4; i++) {
//     
//     thread_join(&threads[i], (void **)(&retval));
//   }
  
  // printf("\n--------= Test for thread_cancel API call =--------\n\n");

  // thread_cancel(&threads[2]);

  exit(0);
}
