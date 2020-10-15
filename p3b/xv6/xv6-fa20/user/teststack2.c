#include "types.h"
#include "user.h"

int main(int argc, char *argv[]) {
  char a[3840+4096*10];
  printf(1, "addr a[0]: %x\n", &a);
  printf(1, "addr a[3840+4096]: %x\n", &a[3840+4096*10]);

  exit();
}