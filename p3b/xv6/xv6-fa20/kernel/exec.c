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
  cprintf("**in exec**\n");
  char *s, *last;
  int i, off;
  uint argc, sz, sp, ustack[3+MAXARG+1];
  struct elfhdr elf;
  struct inode *ip;
  struct proghdr ph;
  pde_t *pgdir, *oldpgdir;

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
    if(loaduvm(pgdir, (char*)ph.va, ip, ph.offset, ph.filesz) < 0)
      goto bad;
  }
  iunlockput(ip);
  ip = 0;

  // Allocate a one-page stack at the next page boundary
  // P3B
  sz = PGROUNDUP(sz); // does sz need to be page aligned?
  uint stackLow, stackHigh;
  stackLow = USERTOP-PGSIZE;
  if((stackHigh = allocuvm(pgdir, stackLow, USERTOP)) == 0)
    goto bad;

  // OLD:
  // sz = PGROUNDUP(sz);
  // if((sz = allocuvm(pgdir, sz, sz + PGSIZE)) == 0)
  //   goto bad;


  // Push argument strings, prepare rest of stack in ustack.
  // P3B
  sp = stackHigh;

  // OLD:
  // sp = sz;

  for(argc = 0; argv[argc]; argc++) {
    if(argc >= MAXARG)
      goto bad;
    sp -= strlen(argv[argc]) + 1;
    cprintf("exec: argv[%d] = %s\n", argc, argv[argc]);
    cprintf("exec: sp after subtracting argc %d = %d\n", argc, sp);
    sp &= ~3;
    cprintf("exec: sp after bit comparison argc %d = %d\n", argc, sp);
    if(copyout(pgdir, sp, argv[argc], strlen(argv[argc]) + 1) < 0)
      goto bad;
    ustack[3+argc] = sp;
    cprintf("exec: sp at end of loop argc %d = %d\n", argc, sp);
  }
  ustack[3+argc] = 0;

  ustack[0] = 0xffffffff;  // fake return PC
  ustack[1] = argc;
  ustack[2] = sp - (argc+1)*4;  // argv pointer

  sp -= (3+argc+1) * 4;
  cprintf("exec: sp after subtracting before copyout = %d\n", sp);
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
  //proc->tf->esp = 0x2000; // testing
  switchuvm(proc);
  freevm(oldpgdir);

  cprintf("exec: sz = %d\n", proc->sz);
  cprintf("exec: esp = %d\n", proc->tf->esp);
  cprintf("exec: eip = %d\n", proc->tf->eip);

  return 0;

 bad:
  if(pgdir)
    freevm(pgdir);
  if(ip)
    iunlockput(ip);
  return -1;
}
