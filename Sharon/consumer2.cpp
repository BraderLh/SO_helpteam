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
#include <signal.h>
#include <string.h>
#include <time.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>


#define MAX_LEN 10000
#define BUFFER_SIZE 10

typedef struct
{
    int value;
} item;

struct region {
    int len;
    char buf[MAX_LEN];
};

struct PIDHora {
    pid_t pid;
    char * tiempo;
};

item buffer[BUFFER_SIZE];

int in = 0;
int out = 0;

struct region * rptr;
struct PIDHora * rptr2;

int fd, fd2;

int main()
{

    time_t tiempo;
    pid_t pidC;

	/* open the shared memory segment */
	fd = shm_open ("/myregion", O_RDONLY,  S_IRUSR | S_IWUSR);
	fd2 = shm_open ( "/PIDHora" , O_RDONLY, S_IRUSR | S_IWUSR );

	if (fd == -1) {
		printf("\n Failed to open shared memory object\n");
		return -1;
	}

	if ( fd2 == -1 ){
        /* Handle error */
        printf("\n Failed to open shared memory object2\n");
        return -1;
    }

	/* now map the shared memory segment in the address space of the process */
	rptr = mmap(NULL, sizeof(struct region), PROT_READ , MAP_SHARED, fd, 0);
	rptr2 = mmap(NULL, sizeof(struct PIDHora), PROT_READ, MAP_SHARED, fd2, 0);

	if ( rptr == MAP_FAILED ){
        /* Handle error */
        printf("\n Failed to map the shared memory 1\n");
        return -1;
    }

	if ( rptr2 == MAP_FAILED ){
        /* Handle error */
        printf("\n Failed to map the shared memory 2\n");
        return -1;
    }
	

	item  next_consumed ;
	
	while( true ) {
		int s = rand() % 5 + 1;

		tiempo = time(0);
        struct tm *tlocal = localtime(&tiempo);
        char output[128];
        strftime(output,128,"%H:%M:%S",tlocal);
	
		printf("%s\n",(char *)rptr->buf);
		
		while( in  ==  out );
		/* do  nothing */
		next_consumed =  buffer [ out ] ;
		out =  ( out + 1) % BUFFER_SIZE ;

		printf("[ %s ] C( %d ) : Matando Proceso %d ( vivió %s ) \n", output , getpid() , rptr2->pid, rptr2->tiempo);

		kill(next_consumed.value , SIGKILL);

		sleep (s);
	}

	return 0;
}
