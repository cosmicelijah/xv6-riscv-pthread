#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "user/threads.h"

// Do nothing, strictly for testing purposes
void *start(void *arg);

int main(int argc, char **argv) {
  thread_t threads[3]; 

  printf("Test for xv6\n");
  printf("\n--------= Test for thread_create API call =--------\n\n");

  int arg = 10;

  thread_create(&threads[0], &start, &arg);

  printf("\n--------= Test for thread_join API call =--------\n\n");

  int retval;

  thread_join(&threads[1], (void **)(&retval));

  printf("\n--------= Test for thread_cancel API call =--------\n\n");

  thread_cancel(&threads[2]);

  return 0;
}
