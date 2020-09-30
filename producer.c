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
#include <sys/time.h>
#include <stdbool.h>
#include <time.h>
#include <stdint.h>
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
	item next_produced;
	item *ptr;
	struct region *rptr;
	int shm_fd;
	
	next_produced.t = time(NULL);
	struct tm *tm; 
	tm = localtime(&(next_produced.t));
	next_produced.id = 0;
	useconds_t sleepTime;
	ptr->in = 0;
	ptr->out = 0;


	/* create the shared memory segment */
	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);

	if (shm_fd == -1)				
	{
		printf("Error\n");
	}
	if (ftruncate(shm_fd, sizeof(item))==-1)
	{
		printf("Error\n");
	}

	/* configure the size of the shared memory segment */
	//ftruncate(shm_fd,sizeof(item));

	/* now map the shared memory segment in the address space of the process */
	rptr = mmap(0, sizeof(item), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if (rptr == MAP_FAILED) {
		printf("Map failed\n");
		return -1;
	}

	//fork();
	
	while (true)
	{
		fork();
		ptr->id=getpid();
		printf("PID %d", ptr->id);
		printf("[%d-%02d-%02d %02d:%02d:%02d]", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
		printf(" P(%d)", ptr->id, ")");
		printf(" Creando proceso %d", ptr->id);
		printf("(%d-%02d-%02d %02d:%02d:%02d)\n", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
		
		while (((ptr->in+1) % BUFFER_SIZE) == ptr->out)
		{
			sleepTime = (useconds_t)((rand() % 5) + 1);
			usleep(sleepTime);
		}
		buffer[ptr->in] = next_produced;
		ptr->in = (ptr->in+1) % BUFFER_SIZE;
	}
	

	/**
	 * Now write to the shared memory region.
 	 *
	 * Note we must increment the value of ptr after each write.
	 */
	/*sprintf(ptr,"%s",message0);
	ptr += strlen(message0);
	printf("Enviando mensage [%s] y durmiendo\n", message0);
	sleep(5);

	sprintf(ptr,"%s",message1);
	ptr += strlen(message1);
	printf("Enviando mensage [%s] y durmiendo\n", message1);
	sleep(5);

	sprintf(ptr,"%s",message2);
	printf("Enviando mensage [%s] y durmiendo\n", message2);
	ptr += strlen(message2);
	sleep(5);*/

	return 0;
}
