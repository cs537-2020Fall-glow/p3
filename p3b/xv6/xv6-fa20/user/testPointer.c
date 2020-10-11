#include "types.h"
#include "user.h"

int main(int argc, char *argv[]) {
  int *p = (int*) 0x2000;
  printf(1, "%x %d\n", p, *p);
  
  exit();
}