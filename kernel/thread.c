#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "defs.h"

int pthread_create(int *thread, void *(*start)(void *), void *arg) {
  printf("This call has not been implemented yet!\n");
  printf("Printed from pthread_create syscall with args:\n");
  printf("\tthread: %p\n", thread);
  printf("\tstart: %p\n", start);
  printf("\targ: %p\n", arg);

  return 0;
}

int pthread_join(int *thread, void **retval) {
  printf("This call has not been implemented yet!\n");
  printf("Printed from pthread_join syscall with args:\n");
  printf("\tthread: %p\n", thread);
  printf("\tretval: %p\n", retval);

  return 0;
}

void pthread_cancel(int *thread) {
  printf("This call has not been implemented yet!\n");
  printf("Printed from pthread_join syscall with args:\n");
  printf("\tthread: %p\n", thread);
}
