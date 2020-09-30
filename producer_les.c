/**
 * Simple program demonstrating shared memory in POSIX systems.
 *
 * This is the producer process that writes to the shared memory region.
 *
 * Figure 3.16
 * 
 * To compile, enter
 * 	gcc shm-posix-producer.c -lrt
 *
 * @author Silberschatz, Galvin, and Gagne
 * Operating System Concepts  - Tenth Edition
 * Copyright John Wiley & Sons - 2018
 */


#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
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
} item;

struct PIDHora {
    int id_pid;
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
	pid_t pid;
	int random;

	/* RPTR */ 
	fd = shm_open(name, O_CREAT | O_RDWR, 0666);
	if (fd == -1) {
		perror("shm open failed\n"); /* Handle error */;
		return -1;
	}

	if (ftruncate(fd, sizeof(struct region)) == -1) {
		perror("ftruncate failed\n") /* Handle error */;
		return -1;
	}

	rptr = mmap(NULL,sizeof(struct region), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	if (rptr == MAP_FAILED) {
		printf("Map failed\n");		/* Handle error */;
		return -1;
	}

	/* PIDPTR */ 

	if (ftruncate(fd, sizeof(struct PIDHora)) == -1) {
		perror("ftruncate failed\n") /* Handle error */;
		return -1;
	}

	pid_ptr = mmap(NULL,sizeof(struct PIDHora), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	if (pid_ptr == MAP_FAILED) {
		printf("Map failed\n");		/* Handle error */;
		return -1;
	}

	/* WHILE */ 

	item next_produced;
	while (true) {
		pid = fork();
		random = rand() % 5 + 1;

		if (pid < 0) { // Error occurred.
			fprintf(stderr, "Fork failed!\n");
			return 1;
		} else if (pid == 0) { // Child process.
			//printf("[FECHA] P(%d): Creando Proceso %d (FECHA)\n",getppid(), getpid());
		} else { // Parent process.
			//printf("[FECHA] C(%d): Matando Proceso %d (vivio TIEMPO)\n",getppid(), getpid());
			printf("[FECHA] P(%d): Creando Proceso %d (FECHA)\n",getppid(), getpid());
			next_produced.pid = getpid();

			printf("%d\n", next_produced.pid);
			//pid_ptr->pid = pid;

			//printf("[FECHA] P(%d): Creando Proceso %d (FECHA)\n",getppid(), getpid());
			
			/* produce an item in next produced */
			while (((in + 1) % BUFFER_SIZE) == out)
			; /* do nothing */
			buffer[in] = next_produced;
			in = (in + 1) % BUFFER_SIZE;

		}

		sleep(random);
	} 


	return 0;
}