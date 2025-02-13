#include "kernel/types.h"

typedef int thread_t;

void thread_exit() {
  printf("Thread exiting!/n");
  pthread_exit();
}

int thread_create(thread_t *thread, void *(*start)(void *), void *arg) {
  int tid = *(int *)thread;
  return pthread_create(tid, start, arg, thread_exit);
}

int thread_join(thread_t *thread, void **retval) {
  int tid = *(int *)thread;
  return pthread_join(tid, retval);
}

void thread_cancel(thread_t *thread) {
  int tid = *(int *)thread;
  pthread_cancel(tid);
}
