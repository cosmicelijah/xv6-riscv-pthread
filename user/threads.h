#ifndef __THREADS_H
#define __THREADS_H

typedef int thread_t;
#define NTHREADS 8

void thread_exit();

int thread_create(thread_t *thread, void *(*start)(void *), void *arg);

int thread_join(thread_t *thread, void **retval);

void thread_cancel(thread_t *thread);

#endif
