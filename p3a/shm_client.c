#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/mman.h>
// ADD NECESSARY HEADERS
#include "stats.h"
#include "signal.h"
#include "string.h"

#define MAX_CLIENTS 63
#define SHM_NAME "/kokesh_low"

// Mutex variables
pthread_mutex_t* mutex;
union seg_t *clients;
void *ptr;

void exit_handler(int sig) {
    // ADD

    // critical section begins
	pthread_mutex_lock(mutex);

    // Client leaving; needs to reset its segment   

	pthread_mutex_unlock(mutex);
	// critical section ends

    exit(0);
}

int main(int argc, char *argv[]) {
	// ADD    
    if (argc != 2) {
        exit(1);
    }

    if (strlen(argv[1]) > 9) {
        exit(1);
    }

	// Open an existing shared memory segment
	int fd_shm = shm_open(SHM_NAME, O_RDWR, 0660);	
	if (fd_shm == -1) {
		exit(1);
	}

	int page_size = getpagesize();

	// Get existing mapping
	ptr = mmap(NULL, page_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_shm, 0);
	if (ptr == MAP_FAILED) {
		exit(1);
	}

    printf("Client ptr = %p\n", ptr);

    mutex = ptr;
	
    // critical section begins
    pthread_mutex_lock(mutex);
        
	// client updates available segment
    union seg_t proc;
    clients = ptr + 64;
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        proc = clients[i];
        // display statistics for each client process
        // [Iteration], pid : [pid], birth : [dateOfBirth], elapsed : [sec] s [msec] ms, [clientString]
        if (proc.stats.pid == 0)
        {
            printf("i = %d\n", i);
            proc.stats.pid = getpid();
            strcpy(proc.stats.clientString, argv[1]);
            proc.stats.elapsed_msec = 0;
            proc.stats.elapsed_sec = 0;
            strcpy(proc.stats.birth, "test"); // TODO


            
            printf("pid : %d, birth : %s, elapsed : %d s %f ms, %s\n", proc.stats.pid, proc.stats.birth, proc.stats.elapsed_sec, proc.stats.elapsed_msec, proc.stats.clientString);
            break;
        }
    }

    pthread_mutex_unlock(mutex);
    // critical section ends
        
	while (1) {
        
		// ADD
        
        sleep(1);

		// Print active clients
    }

    return 0;
}
