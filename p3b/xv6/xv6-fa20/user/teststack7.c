#include "types.h"
#include "user.h"

int main(int argc, char *argv[]) {  
  int *p = (int*) ((640*1024) - (4096*2) - 1);
  printf(1, "Should segfault - one lower than second page adjacent to stack\n");
  printf(1, "%x\n", *p);
  
  exit();
}