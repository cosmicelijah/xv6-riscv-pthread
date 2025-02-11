#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "defs.h"

// struct spinlock wait_lock;

// struct proc proc[NPROC];

// extern void threadret(void);

// // free a proc structure and the data hanging from it,
// // including user pages.
// // p->lock must be held.
// static void
// freethread(struct proc *p)
// {
//   if(p->trapframe)
//     kfree((void*)p->trapframe);
//   p->trapframe = 0;
//   if(p->pagetable)
//     proc_freepagetable(p->pagetable, p->sz);
//   p->pagetable = 0;
//   p->sz = 0;
//   p->pid = 0;
//   p->parent = 0;
//   p->name[0] = 0;
//   p->chan = 0;
//   p->killed = 0;
//   p->xstate = 0;
//   p->state = UNUSED;
//   p->is_thread = 0;
//   p->tid = 0;
// }

// // Look in the process table for an UNUSED proc.
// // If found, initialize state required to run in the kernel,
// // and return with p->lock held.
// // If there are no free procs, or a memory allocation fails, return 0.
// static struct proc* allocthread(void){
//   struct proc *p;

//   for(p = proc; p < &proc[NPROC]; p++) {
//     acquire(&p->lock);
//     if(p->state == UNUSED) {
//       goto found;
//     } else {
//       release(&p->lock);
//     }
//   }
//   return 0;

// found:
//   // PID is set in pthread_create
//   p->state = USED;
//   //josiah's additions
//   p->is_thread = 1;

//   // Allocate a trapframe page.
//   if((p->trapframe = (struct trapframe *)kalloc()) == 0){
//     freethread(p);
//     release(&p->lock);
//     return 0;
//   }

//   // An empty user page table.
//   p->pagetable = proc_pagetable(p);
//   if(p->pagetable == 0){
//     freethread(p);
//     release(&p->lock);
//     return 0;
//   }

//   // Set up new context to start executing at forkret,
//   // which returns to user space.
//   memset(&p->context, 0, sizeof(p->context));
//   p->context.ra = (uint64)threadret;
//   p->context.sp = p->kstack + PGSIZE;

//   return p;
// }

// void threadret(void)
// {
//   static int first = 1;

//   // Still holding p->lock from scheduler.
//   release(&myproc()->lock);

//   if (first) {
//     // File system initialization must be run in the context of a
//     // regular process (e.g., because it calls sleep), and thus cannot
//     // be run from main().
//     fsinit(ROOTDEV);

//     first = 0;
//     // ensure other cores see first=0.
//     __sync_synchronize();
//   }

//   usertrapret();
// }

// void pthread_cancel(int tid) {
//   printf("This call has not been implemented yet!\n");
//   printf("Printed from pthread_join syscall with args:\n");
//   printf("\tthread: %d\n", tid);
// }

// //pthread stuff made by josiah with elijah:
// int pthread_create(int tid, void*(*start)(void*), void*arg) {
//   printf("Printed from pthread_create syscall with args:\n");
//   printf("\tthread: %d\n", tid);
//   printf("\tstart: %p\n", start);
//   printf("\targ: %p\n", arg);

//   int i;
//   struct proc *t;
//   struct proc *p = myproc();

//   // Allocate process.
//   if((t = allocthread()) == 0){
//     return -1;
//   }

//   // Copy user memory from parent to child.
//   if(uvmcopy(p->pagetable, t->pagetable, p->sz) < 0){
//     freethread(t);
//     release(&t->lock);
//     return -1;
//   }
//   t->sz = p->sz;
  
//   //set the threads pid and tid
//   t->pid = p->pid;
//   t->tid = tid;

//   // copy saved user registers and copy epc to start at start function
//   *(t->trapframe) = *(p->trapframe);
//   t->trapframe->epc = (uint64)start;
  

//   // Set args in trapframe to whats given.
//   t->trapframe->a0 = (uint64)arg;

//   // increment reference counts on open file descriptors.
//   for(i = 0; i < NOFILE; i++)
//     if(p->ofile[i])
//       t->ofile[i] = filedup(p->ofile[i]);
//   t->cwd = idup(p->cwd);

//   safestrcpy(t->name, p->name, sizeof(p->name));

//   release(&t->lock);

//   acquire(&wait_lock);
//   t->parent = p;
//   release(&wait_lock);

//   acquire(&t->lock);
//   t->state = RUNNABLE;
//   release(&t->lock);

//   //changes the return address the 
//   t->context.ra = (uint64)threadret;

//   // If here, all good, return 0 for success
//   return 0;
// }

// int pthread_join(int tid, void**retval) {
  
//     // struct proc *p = myproc();
//     // struct proc *t;

//     // acquire(&wait_lock);

//     // for (;;) {
//     //     int found = 0;
//     //     for (t = proc; t < &proc[NPROC]; t++) {
//     //         if (t->pid == *thread && t->parent == p) {
//     //             found = 1;

//     //             acquire(&t->lock);
//     //             if (t->state == ZOMBIE) {
//     //                 freeproc(t);
//     //                 release(&t->lock);
//     //                 release(&wait_lock);
//     //                 return *thread;
//     //             }
//     //             release(&t->lock);
//     //         }
//     //     }

//     //     if (!found) {
//     //         release(&wait_lock);
//     //         return -1;
//     //     }

//     //     sleep(p, &wait_lock);
//     // }
//     return 0;
// }
// //end of josiah's stuff with elijah

