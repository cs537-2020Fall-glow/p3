#include <stdio.h>
#include "string.h"
#include "time.h"

int main(int argc, char *argv[]) {
  printf("struct timespec: %ld\n", sizeof(struct timespec));
  printf("clock_t: %ld\n", sizeof(clock_t));
  
}