#include "kernel/types.h"

typedef int thread_t;

int thread_create(thread_t *thread, void *(*start)(void *), void *arg) {
  return pthread_create((int *)thread, start, arg);
}

int thread_join(thread_t *thread, void **retval) {
  return pthread_join((int *)thread, retval);
}

void thread_cancel(thread_t *thread) {
  pthread_cancel((int *)thread);
}
