CC=gcc -std=gnu99
CFLAGS= -Wall -pedantic -g
OBJ= main.o createthread.o matrixMult.o
EXEC= pmms

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) -lpthread

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

createthread.o: createthread.c
	$(CC) $(CFLAGS) -c createthread.c

matrixMult.o: matrixMult.c
	$(CC) $(CFLAGS) -c matrixMult.c

clean:
	rm -f $(OBJ) $(EXEC)
