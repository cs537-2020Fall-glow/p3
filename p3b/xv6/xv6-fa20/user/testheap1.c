#include "types.h"
#include "user.h"

int main(int argc, char *argv[]) {
  char *string = malloc(sizeof(int));
  if (string == NULL) {
    printf(1, "malloc failed\n");
    exit();
  }
  
  printf(1, "string addr: %x\n", string);
  
  exit();
}