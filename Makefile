all: program1 program2

program1: 
	gcc producer.c -lrt -o producer

program2:
	gcc consumer.c -lrt -o consumer

RunAll: runProgram1 runProgram2
	./producer
	./consumer