#include "types.h"
#include "user.h"

int main(int argc, char *argv[]) {
  int *p = (int*) 0x9000;
  printf(1, "in unmapped space between stack and heap, more than one page away from stack lower addr\n");
  printf(1, "%x\n", *p);
  
  exit();
}