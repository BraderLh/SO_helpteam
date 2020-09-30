# Práctica 1 de Laboratiro

Esta práctica tratará sobre como crear y finalizar procesos usando una memoria compartida y comunicación entre procesos.

## Solución:
### Definición de la estructura:
Para la resolución de este problema se utilizó una memoria compartida el cual utiliza un tipo de estructura llamada 'struct'

```c
typedef struct {
    int id;
	int in;
	int out;
	time_t t;
}item;
item buffer[BUFFER_SIZE];
```
Se declaró ***id, in, out*** y ***time_t t*** como datos miembros principales de la estructura. El **id** es para guardar el número del proceso generado por la memoria compartida, respecto a **in** y **out** van a ser los índices para funcionar como punteros a lista de procesos que se generará al momento de crear el primer proceso. Y el último **time_t** es para mostrar la fecha y hora completa del proceso creado.

### Generación del proceso en producer:
```c
	while (true)
	{
		rptr->id=getpid();
		printf("PID %d", rptr->id);
		printf("[%d-%02d-%02d %02d:%02d:%02d]", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
		printf(" P(%d)", rptr->id, ")");
		printf(" Creando proceso %d", rptr->id);
		printf("(%d-%02d-%02d %02d:%02d:%02d)\n", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
		
		while (((rptr->in+1) % BUFFER_SIZE) == rptr->out)
		{
			sleepTime = (useconds_t)((rand() % 5) + 1);
			usleep(sleepTime);
		}
		buffer[rptr->in] = next_produced;
		rptr->in = (rptr->in+1) % BUFFER_SIZE;
	}
```
### Recepción del proceso proceso en consumer:
```c
	while (true)
	{
		while (rptr->in == rptr->out)
		{
			sleepTime = (useconds_t)((rand() % 5) + 1);
			usleep(sleepTime);
		}
		
		next_consumed = buffer[rptr->out];
		rptr->out = (rptr->out + 1) % BUFFER_SIZE;
		//(hora_anterior=tm->tm_hour; 
		//kill(rptr->id, SIGKILL);
		printf("[%d-%02d-%02d %02d:%02d:%02d]", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
		printf(" C(%d)", rptr->id, ")");
		printf(" Matando proceso %d", rptr->id);
		kill(rptr->id, SIGKILL);
		//printf("(%d-%02d-%02d %02d:%02d:%02d)\n", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);

	}
```

### Compilación:
Para compilar solo ejecutar el archivo Makefile con el comando ```make all ``` 
y eso es todo. O también puede abrir usted dos terminales y ejecutar ambos en simultáneo.