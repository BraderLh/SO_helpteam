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
#include <time.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>

#define MAX_LEN 10000
#define BUFFER_SIZE 10

typedef struct{
    int value;
} item;

struct region {
    int len;
    char buf[MAX_LEN];
};

struct PIDHora {
    pid_t pid;
    char *tiempo;
};

item buffer[BUFFER_SIZE];

int in = 0;
int out = 0;

struct region * rptr;
struct PIDHora * rptr2;

int fd, fd2;

int main(){

    time_t tiempo;
    pid_t pidC;

    /* Create shared memory object and set its size */
    fd = shm_open ( "/myregion" , O_CREAT | O_RDWR, S_IRUSR | S_IWUSR );
    fd2 = shm_open ( "/PIDHora" , O_CREAT | O_RDWR, S_IRUSR | S_IWUSR );

    if ( fd == -1 ){
        /* Handle error */
        printf("\n Failed to open shared memory object1\n");
        return -1;
    }

    if ( fd2 == -1 ){
        /* Handle error */
        printf("\n Failed to open shared memory object2\n");
        return -1;
    }

    if ( ftruncate(fd, sizeof( struct region)) == -1 ){
        /* Handle error */
        printf("\n Failed to set the size of the shared memory object1\n");
        return -1;
    }

    if ( ftruncate(fd2, sizeof( struct PIDHora)) == -1 ){
        /* Handle error */
        printf("\n Failed to set the size of the shared memory object2\n");
        return -1;
    }

    /* Map shared memory object */
    rptr = mmap(NULL, sizeof(struct region), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    rptr2 = mmap(NULL, sizeof(struct PIDHora), PROT_READ | PROT_WRITE, MAP_SHARED, fd2, 0);

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


    /* WHILE */ 

    item next_produced;

    while(true){

        pidC = fork();

        int s = rand() % 5 + 1;

        tiempo = time(0);
        struct tm *tlocal = localtime(&tiempo);
        char output[128];
        strftime(output,128,"%H:%M:%S",tlocal);

        rptr2->pid = pidC;
        rptr2->tiempo = output;

        printf("[ %s ] P( %d ) : Creando Proceso %d ( %s ) \n", rptr2->tiempo, getpid() ,pidC, output);

        next_produced.value = getpid();

        while((( in + 1 ) % BUFFER_SIZE ) == out)
            ;
        buffer[in] = next_produced;
        in = ( in + 1) % BUFFER_SIZE;
        
        sleep(s);
    }
    
    return 0;
}







