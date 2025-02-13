#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64
sys_pthread_create(void) 
{
  int tid;
  uint64 start;
  uint64 arg;
  uint64 exit;
  
  argint(0, &tid);
  argaddr(1, &start);
  argaddr(2, &arg);
  argaddr(3, &exit);
  return pthread_create(tid, (void*(*)(void*))start, (void*)arg, (void(*)(void))exit);
}

uint64
sys_pthread_join(void)
{
  int tid;
  uint64 retval;

  argint(0, &tid);
  argaddr(1, &retval);

  return pthread_join(tid, (void **)retval);
}

void
sys_pthread_cancel(void)
{
  int tid;

  argint(0, &tid);

  pthread_cancel(tid);
}

void
sys_pthread_exit(void)
{
  pthread_exit();
}
