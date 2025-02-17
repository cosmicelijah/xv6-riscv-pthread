#include "kernel/types.h"
#include "user/threads.h"
#include "user/user.h"

static thread_t __threads[NTHREADS] = {0};

void thread_exit() {
  pthread_exit();
}

int thread_create(thread_t *thread, void *(*start)(void *), void *arg) {
  int tid = -1;
  
  for (int i = 0; i < NTHREADS; i++) {
  	if (__threads[i] == 0) {
  	  tid = i + 1;
  	  __threads[i] = 1;
  	  break;
  	}
  }

  // No thread slots left
  if (tid == -1) return tid;

  *thread = tid;
  
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
