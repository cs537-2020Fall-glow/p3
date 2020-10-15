#include "types.h"
#include "user.h"

int main(int argc, char *argv[]) {  
  char a[4096*160];
  printf(1, "Should segfault - ensure 5 unmapped pages between stack and heap\n");
  printf(1, "addr a[4096*160]: %x\n", &a[4096*160]); // shouldn't print
  
  exit();
}