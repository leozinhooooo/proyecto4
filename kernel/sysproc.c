#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "vm.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  kexit(n);
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
  return kfork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return kwait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);

  struct proc *p = myproc();
  addr = p->sz;

  if(n < 0)
    return -1;

  if(addr + n < addr)
    return -1;

  if(addr + n > TRAPFRAME)
    return -1;

  p->sz += n;

  return addr;
}

uint64
sys_pause(void)
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
  return kkill(pid);
}

// return how many clock tick interrupts have occurred
// since start.


//funcion de la mapzero
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
sys_mapzero(void)
{
  int size;
  uint64 addr;

  argint(0, &size);

  addr = myproc()->sz;

  myproc()->vregion.start = addr;
  myproc()->vregion.size = size;

  myproc()->sz += size;

  return addr;
}
uint64
sys_mmap(void)
{
    uint64 addr;
    uint64 length;
    int prot;
    int flags;
    int fd;
    uint64 offset;

    struct proc *p = myproc();

    argaddr(0, &addr);
    argaddr(1, &length);
    argint(2, &prot);
    argint(3, &flags);
    argint(4, &fd);
    argaddr(5, &offset);

    // validar
    if(length <= 0)
        return -1;

    if(fd < 0 || fd >= NOFILE || p->ofile[fd] == 0)
        return -1;

    // buscar VMA libre
    struct vma *v = 0;

    for(int i = 0; i < MAXVMA; i++){
        if(p->vmas[i].used == 0){
            v = &p->vmas[i];
            break;
        }
    }

    if(v == 0)
        return -1;

    // llenar VMA
    v->used = 1;

    v->addr = p->sz;
    v->length = length;

    v->prot = prot;
    v->flags = flags;

    v->file = filedup(p->ofile[fd]);

    v->offset = offset;

    // reservar espacio virtual
    p->sz += length;

    return v->addr;
}

uint64
sys_munmap(void)
{
    uint64 addr;

    argaddr(0, &addr);

    struct proc *p = myproc();

    for(int i = 0; i < MAXVMA; i++){

        struct vma *v = &p->vmas[i];

        if(v->used && v->addr == addr){

            uvmunmap(p->pagetable,
                     addr,
                     PGROUNDUP(v->length) / PGSIZE,
                     1);

            v->used = 0;

            fileclose(v->file);

            return 0;
        }
    }

    return -1;
}
