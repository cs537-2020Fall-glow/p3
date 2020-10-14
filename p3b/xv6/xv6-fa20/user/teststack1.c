#include "types.h"
#include "user.h"

int main(int argc, char *argv[]) {
  char a[3840];
  printf(1, "addr a[0]: %x\n", &a);
  printf(1, "addr a[3840]: %x\n", &a[3840]);
  printf(1, "anymore and we seg fault upon running test\n");

  exit();
}