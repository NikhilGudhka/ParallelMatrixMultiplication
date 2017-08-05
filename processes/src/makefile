CC=gcc -std=gnu99
CFLAGS= -Wall -pedantic -g
OBJ=main.o sharedmem.o matrixMult.o readFile.o
EXEC= pmms

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) -lrt -lpthread

matrixMult.o: matrixMult.c
	$(CC) -c matrixMult.c $(CFLAGS)

main.o: main.c
	$(CC) -c main.c $(CFLAGS)

sharedmem.o: sharedmem.c
	$(CC) -c sharedmem.c $(CFLAGS)

readFile.o: readFile.c
	$(CC) -c readFile.c $(CFLAGS)

clean:
	rm -f $(EXEC) $(OBJ)
