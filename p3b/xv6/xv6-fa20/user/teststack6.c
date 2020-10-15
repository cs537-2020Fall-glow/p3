#include "types.h"
#include "user.h"

int main(int argc, char *argv[]) {  
  int *p2 = (int*) ((640*1024) - (4096*2));
  printf(1, "border of second page adjacent to stack\n");
  printf(1, "%x\n", *p2);
  
  exit();
}