#include "types.h"
#include "riscv.h"
#include "param.h"
#include "spinlock.h"
#include "defs.h"
#include "sysinfo.h"
#include "proc.h"

int sinfo(uint64 addr)
{
  struct proc *p = myproc();
  struct sysinfo si;

  si.freemem = freeamt();
  si.nproc = numprocs();

  if (copyout(p->pagetable, addr, (char *)&si, sizeof(si)) < 0)
    return -1;

  return 0;
}
