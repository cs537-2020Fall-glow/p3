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
#include "time.h"

#define MAX_CLIENTS 63
#define SHM_NAME "/kokesh_low"

// Mutex variables
pthread_mutex_t* mutex;
union seg_t *clients;
void *ptr;
union seg_t *proc;

void exit_handler(int sig) {

    // critical section begins
	pthread_mutex_lock(mutex);

    // Client leaving; needs to reset its segment   
    for (int i = 0; i < MAX_CLIENTS; i++) {
	    proc = &clients[i];
		if (proc->stats.pid == getpid()) {
			proc->stats.pid = 0;
            proc->stats.elapsed_msec = 0;
            proc->stats.elapsed_sec = 0;
            proc->stats.start_time = 0;
            memset(proc->stats.birth, 0, sizeof(proc->stats.birth));
            memset(proc->stats.clientString, 0, sizeof(proc->stats.clientString));
		}
	}

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

    mutex = ptr;
	
    // critical section begins
    pthread_mutex_lock(mutex);
        
	// client updates available segment
    clients = ptr + 64;
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        proc = &clients[i];
        // display statistics for each client process
        // [Iteration], pid : [pid], birth : [dateOfBirth], elapsed : [sec] s [msec] ms, [clientString]
        if (proc->stats.pid == 0)
        {
            proc->stats.pid = getpid();
            strcpy(proc->stats.clientString, argv[1]);
            proc->stats.elapsed_msec = 0;
            proc->stats.elapsed_sec = 0;
            proc->stats.start_time = clock();
            time_t current_time = time(NULL);
            char *c_time = ctime(&current_time);
            c_time[24] = '\0';
            memcpy(proc->stats.birth, c_time, sizeof(proc->stats.birth));

            break;
        }
    }

    pthread_mutex_unlock(mutex);
    // critical section ends

    struct sigaction act;
	act.sa_handler = exit_handler;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
	sigaction(SIGINT, &act, NULL);
	sigaction(SIGTERM, &act, NULL);
        
	while (1) {
        
        sleep(1);

		// Print active clients
        printf("Active clients :");
        
        proc = NULL;
		for (int i = 0; i < MAX_CLIENTS; i++) {
			proc = &clients[i];
			if (proc->stats.pid != 0) {
				printf(" %d", proc->stats.pid);
			}
            if (proc->stats.pid == getpid()) {
                clock_t cur_time = clock();
                //printf("cur_time = %ld\n", cur_time);
                double total_sec = (double) (cur_time - proc->stats.start_time) / CLOCKS_PER_SEC;
                //printf("total_sec = %f\n", total_sec);
                proc->stats.elapsed_sec = (int) total_sec;
                //printf("elapsed_sec = %d\n", proc->stats.elapsed_sec);
                proc->stats.elapsed_msec = ((total_sec - proc->stats.elapsed_sec) * 1000);
                //printf("elapsed_msec = %f\n", proc->stats.elapsed_msec);
            }
		}
        printf("\n");
    }

    return 0;
}
