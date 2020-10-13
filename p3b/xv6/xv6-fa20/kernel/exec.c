#include "types.h"
#include "param.h"
#include "mmu.h"
#include "proc.h"
#include "defs.h"
#include "x86.h"
#include "elf.h"

int
exec(char *path, char **argv)
{
  char *s, *last;
  int i, off;
  uint argc, sz, sp, ustack[3+MAXARG+1];
  struct elfhdr elf;
  struct inode *ip;
  struct proghdr ph;
  pde_t *pgdir, *oldpgdir;
  // cprintf("test\n"); // P3B

  if((ip = namei(path)) == 0)
    return -1;
  ilock(ip);
  pgdir = 0;

  // Check ELF header
  if(readi(ip, (char*)&elf, 0, sizeof(elf)) < sizeof(elf))
    goto bad;
  if(elf.magic != ELF_MAGIC)
    goto bad;

  if((pgdir = setupkvm()) == 0)
    goto bad;

  // cprintf("before load\n"); // P3B
  // Load program into memory.
  sz = 0x2000; // P3B
  for(i=0, off=elf.phoff; i<elf.phnum; i++, off+=sizeof(ph)){
    if(readi(ip, (char*)&ph, off, sizeof(ph)) != sizeof(ph))
      goto bad;
    if(ph.type != ELF_PROG_LOAD)
      continue;
    if(ph.memsz < ph.filesz)
      goto bad;
    if((sz = allocuvm(pgdir, sz, ph.va + ph.memsz)) == 0)
      goto bad;
    // cprintf("ph.va: %x, offset: %d, filesz: %d\n", (char*)ph.va, ph.offset, ph.filesz); // P3B
    if(loaduvm(pgdir, (char*)ph.va, ip, ph.offset, ph.filesz) < 0)
      goto bad;
  }
  iunlockput(ip);
  ip = 0;
  
  // cprintf("afterload\n"); // P3B
  
  // Allocate a one-page stack at the next page boundary
  // P3B
  uint stackLow, stackHigh;
  stackLow = USERTOP-PGSIZE;
  cprintf("%x\n",stackLow);
  sz = PGROUNDUP(sz);
  cprintf("%x\n",stackLow);
  if((stackHigh = allocuvm(pgdir, stackLow, USERTOP)) == 0)
    goto bad;
  cprintf("%x\n",stackLow);
  cprintf("%x\n",stackHigh);

  // Push argument strings, prepare rest of stack in ustack.
  // P3B
  sp = stackHigh;
  for(argc = 0; argv[argc]; argc++) {
    if(argc >= MAXARG)
      goto bad;
    sp -= strlen(argv[argc]) + 1;
    sp &= ~3;
    cprintf("argc: %d, sp: %x\n", argc, sp);
    if(copyout(pgdir, sp, argv[argc], strlen(argv[argc]) + 1) < 0)
      goto bad;
    ustack[3+argc] = sp;
  }
  ustack[3+argc] = 0;

  ustack[0] = 0xffffffff;  // fake return PC
  ustack[1] = argc;
  ustack[2] = sp - (argc+1)*4;  // argv pointer

  sp -= (3+argc+1) * 4;
  if(copyout(pgdir, sp, ustack, (3+argc+1)*4) < 0)
    goto bad;

  // Save program name for debugging.
  for(last=s=path; *s; s++)
    if(*s == '/')
      last = s+1;
  safestrcpy(proc->name, last, sizeof(proc->name));

  // Commit to the user image.
  oldpgdir = proc->pgdir;
  proc->pgdir = pgdir;
  proc->sz = sz;
  proc->stackLow = stackLow; // P3B
  proc->tf->eip = elf.entry;  // main
  proc->tf->esp = sp;
  switchuvm(proc);
  freevm(oldpgdir);
  
  // P3B - testing
  cprintf("sp: %x\n", sp);
  // cprintf("ustack[0]: %x\n", &ustack[0]);
  // cprintf("ustack[1]: %x\n", &ustack[1]);
  // cprintf("ustack[2]: %x\n", &ustack[2]);

  return 0;

 bad:
  if(pgdir)
    freevm(pgdir);
  if(ip)
    iunlockput(ip);
  return -1;
}
