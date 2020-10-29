#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include "signal.h"
#include "stats.h"

#define MAX_CLIENTS 63
#define SHM_NAME "/kokesh_low"

union seg_t *clients;
void *ptr;

// Mutex variables
pthread_mutex_t *mutex;
pthread_mutexattr_t mutexAttribute;

void exit_handler(int sig) {
  munmap(ptr, getpagesize());
  shm_unlink(SHM_NAME);

  exit(0);
}

int main(int argc, char *argv[]) {
  // Creating a new shared memory segment
  int fd_shm = shm_open(SHM_NAME, O_RDWR | O_CREAT, 0660);
  if (fd_shm == -1) {
    exit(1);
  }

  int page_size = getpagesize();
  ftruncate(fd_shm, page_size);

  // Create new mapping
  ptr = mmap(NULL, page_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_shm, 0);
  if (ptr == MAP_FAILED) {
    exit(1);
  }

  // Initializing mutex
  pthread_mutexattr_init(&mutexAttribute);
  pthread_mutexattr_setpshared(&mutexAttribute, PTHREAD_PROCESS_SHARED);
  mutex = ptr;
  pthread_mutex_init(mutex, &mutexAttribute);

  int iteration = 0;
  clients = ptr + 64;

  struct sigaction act;
  act.sa_handler = exit_handler;
  act.sa_flags = 0;
  sigemptyset(&act.sa_mask);
  sigaction(SIGINT, &act, NULL);
  sigaction(SIGTERM, &act, NULL);

  while (1) {
    iteration++;

    // read contents of shared memory
    union seg_t *proc;
    for (int i = 0; i < MAX_CLIENTS; i++) {
      proc = &clients[i];
      // display statistics for each client process
      if (proc->stats.pid != 0) {
        printf("%d, pid : %d, birth : %s, elapsed : %d s %.4f ms, %s\n",
               iteration, proc->stats.pid, proc->stats.birth,
               proc->stats.elapsed_sec, proc->stats.elapsed_msec,
               proc->stats.clientString);
      }
    }

    sleep(1);
  }

  return 0;
}
