#include "types.h"
#include "user.h"

int main(int argc, char *argv[]) {
  // char a[4096];
  // printf(1, "first stack addr: %x\n", &a);
  // printf(1, "last stack addr: %x\n", &a[4096]);
  char a;
  char b;
  printf(1, "argc: %x\n", &argc);
  printf(1, "a: %x\n", &a);
  printf(1, "b: %x\n", &b);
  char *ptrToStack = (char*) 0x3FFF;
  printf(1, "byte at 0x3FFF: %d\n", *ptrToStack);
  
  exit();
}