#include <stdio.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <sys/time.h> // for struct timeval
#include <fcntl.h>
#include <unistd.h>   // for ftruncate()
#include <stdlib.h>   // for srand() rand()
#include <sys/wait.h> // for wait()

#define BUFSIZE 5

struct region
{
    int in;
    int out;
    int buffer[BUFSIZE];
};

int main()
{
    
    struct region *rptr;
    int fd;

    int pid;
    int i;
    int duration;
    
    fd=shm_open("/myregion", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	if(fd==-1)
	{
		printf("Map failed\n");
		return -1;
	}

	if(ftruncate(fd, sizeof(struct region)) == 1)
	{
		printf("Map failed\n");
		return -1;
	}

	rptr=mmap(NULL, sizeof(struct region), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if(rptr == MAP_FAILED)
	{
		printf("Map failed\n");
		return -1;
	}

    rptr->in = 0;
    rptr->out = 0;

    while(true)
    {
        duration = (rand())%7;
        pid = fork(); 
        if(pid==0)
        {
            i=getpid();
            printf("P(%d):Creando Proceso %d)\n",getppid(), getpid());
        }
        while (((rptr->in+1)%BUFSIZE) == rptr->out) 
            /* do nothing! */ ;
        rptr->buffer[rptr->in] = i;
        rptr->in = (rptr->in+1)%BUFSIZE;
        sleep(duration);
    }
    
    return 0;
}

