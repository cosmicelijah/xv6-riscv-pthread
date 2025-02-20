#ifndef __THREADS_H
#define __THREADS_H

typedef int thread_t;

enum ThreadState { UNUSED, USED, ZOMBIE };

struct thread_data {
  thread_t tid;				// Unique id for the thread
  void *stack;				// Pointer to the thread's stack
  void *xstate;				// Return value from thread
  enum ThreadState state;	// Current state of the thread
};

#define NTHREADS 8

void thread_exit();

int thread_create(thread_t *thread, void *(*start)(void *), void *arg);

int thread_join(thread_t *thread, void **retval);

void thread_cancel(thread_t *thread);

#endif
