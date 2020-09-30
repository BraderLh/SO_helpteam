/**
 * Simple program demonstrating shared memory in POSIX systems.
 *
 * This is the consumer process
 *
 * Figure 3.18
 *
 * To compile, enter
 *	gcc shm-posix-consumer.c -lrt
 *
 * @author Gagne, Galvin, Silberschatz
 * Operating System Concepts - Tenth Edition
 * Copyright John Wiley & Sons - 2018
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <string.h>
#include <stdbool.h>

#include <signal.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>

#define MAX_LEN 10000
#define BUFFER_SIZE 10

struct region
{
	int len;
	char buf[MAX_LEN];
};

typedef struct {
	int pid;
	pid_t ptr_pid;
} item;

struct PIDHora {
    pid_t pid;
    char * tiempo;
};

item buffer[BUFFER_SIZE];

int in = 0;
int out = 0;

struct region *rptr;
struct PIDHora *pid_ptr;
int fd;

int main()
{
	const char *name = "OS";
	int random;

	/* RPTR */ 
	fd = shm_open(name, O_RDONLY, 0666);
	if (fd == -1) {
		printf("shared memory failed\n");
		return -1;
	}
	
	rptr = mmap(NULL,sizeof(struct region), PROT_READ, MAP_SHARED, fd, 0);
	if (rptr == MAP_FAILED) {
		printf("Map failed\n");
		return -1;
	}

	/* PIDPTR */ 

	pid_ptr = mmap(NULL,sizeof(struct region), PROT_READ, MAP_SHARED, fd, 0);
	if (pid_ptr == MAP_FAILED) {
		printf("Map failed\n");
		return -1;
	}

	/* WHILE */ 

	item next_consumed;
	while (true) {
		random = rand() % 5 + 1;

		while (in == out)
		; /* do nothing */
		next_consumed = buffer[out];
		printf("[FECHA] C(%d): Matando Proceso %d (vivio TIEMPO)\n",getpid(), next_consumed.pid);

		out = (out + 1) % BUFFER_SIZE;
		/* consume the item in next consumed */
		kill(next_consumed.pid, SIGKILL);
		sleep(random);
	} 

	return 0;
}
