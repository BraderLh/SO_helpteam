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
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>
#include <signal.h>
#define BUFFER_SIZE 10

typedef struct {
	int id;
	int in;
	int out;
	time_t t;
	
}item;
item buffer[BUFFER_SIZE];

int main()
{
	const char *name = "OS";
	int shm_fd;
	struct item *rptr;
	item *ptr;
	useconds_t sleepTime;
	item next_consumed;

	next_consumed.t = time(NULL);
	struct tm *tm; 
	tm = localtime(&(next_consumed.t));
	next_consumed.id = 0;
	ptr->in = 0;
	ptr->out = 0;

	/* open the shared memory segment */
	shm_fd = shm_open(name, O_RDONLY, 0666);
	if (shm_fd == -1) {
		printf("shared memory failed\n");
		exit(-1);
	}

	/* now map the shared memory segment in the address space of the process */
	rptr = mmap(0,sizeof(item), PROT_READ, MAP_SHARED, shm_fd, 0);
	if (rptr == MAP_FAILED) {
		printf("Map failed\n");
		exit(-1);
	}

	if (shm_unlink(name) == -1) {
		printf("Error removing %s\n",name);
		exit(-1);
	}

/*
	while (true)
	{
		while (ptr->in == ptr->out)
		{
			sleepTime = (useconds_t)((rand() % 5) + 1);
			usleep(sleepTime);
		}
		next_consumed = buffer[ptr->out];
		ptr->out = (ptr->out + 1) % BUFFER_SIZE;
		//(hora_anterior=tm->tm_hour; 
		//kill(rptr->id, SIGKILL);
		printf("[%d-%02d-%02d %02d:%02d:%02d]", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
		printf(" C(%d)", ptr->id, ")");
		printf(" Matando proceso %d", ptr->id);
		kill(ptr->id, SIGKILL);
		//printf("(%d-%02d-%02d %02d:%02d:%02d)\n", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);

	}
*/


	return 0;
}
