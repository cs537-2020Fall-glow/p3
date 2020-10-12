#include "types.h"
#include "user.h"

int main(int argc, char *argv[]) {
  int *p = 0;
  printf(1, "cause seg fault\n");
  printf(1, "%x", *p);
  
  exit();
}