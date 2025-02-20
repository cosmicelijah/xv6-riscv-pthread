#include "kernel/types.h"
#include "user/threads.h"
#include "user/user.h"

static struct thread_data __threads[NTHREADS] = {0};

static int first = 1;

void thread_exit() {
  // Get return value from thread
  // Do this FIRST to avoid having a0 overwritten
  void *retval;
  asm volatile("mv %0, a0" : "=r"(retval));

  // Get tid of thread that just landed here
  int tid = gettid();

  // Change thread's state to zombie for our purposes.
  // Store return value in our thread metadata
  // Don't free anything yet, just like in kernel exit().
  struct thread_data* td;
  for (td = __threads; td < &__threads[NTHREADS]; td++) {
  	if (td->tid == tid) {
  	  td->state = ZOMBIE;
  	  td->xstate = retval;
  	  break;
  	}
  }

  // Call pthread_exit to clean up the process on the kernel side
  pthread_exit(0);
}

int thread_create(thread_t *thread, void *(*start)(void *), void *arg) {
  int tid = -1;

  // First time initialization
  if (first) {
  	struct thread_data* td;
    for (td = __threads; td < &__threads[NTHREADS]; td++) {
   	  td->tid = -1;
   	  td->state = UNUSED;
   	  td->xstate = 0;
   	  td->stack = 0;
    }
    first = 0;
  }
  
  for (int i = 0; i < NTHREADS; i++) {
  	if (__threads[i].state == UNUSED) {
  	  tid = i;
  	  __threads[i].state = USED;
  	  break;
  	}
  }

  // No thread slots left
  if (tid == -1) return -1;

  // Allocate space for the thread's stack (1 page)
  void *thread_stack = malloc(4096);

  // Fulfill promise to caller
  *thread = tid;

  // Populate thread data
  __threads[tid].tid = tid;
  __threads[tid].stack = thread_stack;
  __threads[tid].xstate = 0; // Flush return value
  
  return pthread_create(tid, start, arg, thread_exit, thread_stack);
}

// Joins a specific thread given by thread_t *thread
// Return value (where necessary) is stored in void **retval
// Returns 0 on success and -1 on failure
int thread_join(thread_t *thread, void **retval) {
  int tid = *(int *)thread;

  // Find thread first and verify its not UNUSED
  int found = -1;
  struct thread_data* td;
  for (td = __threads; td < &__threads[NTHREADS]; td++) {
   	if (td->tid == tid && td->state != UNUSED) {
   	  found = 0;
   	  break;
   	}
  }

  if (found) {
    printf("thread_join: tried to join unallocated thread\n");
  	return -1;
  }

  // Do actual join before freeing the thread slot to avoid race condition
  int success = pthread_join(tid, retval);
  if (success == -1) {
    printf("thread_wait: no threads to wait for\n");
    return -1;
  } else if (success == 1) {
  	printf("thread_wait: error in joining\n");
  }

  if (td->state != ZOMBIE) {
    printf("Joined non zombie thread!\n");
  }
  
  // Free thread's stack
  free(td->stack);

  // Thread is completely gone and ready to be reused
  td->state = UNUSED;

  // Fulfill return value
  *retval = td->xstate;
  
  return success;
}

void thread_cancel(thread_t *thread) {
  int tid = *(int *)thread;
  pthread_cancel(tid);
}
