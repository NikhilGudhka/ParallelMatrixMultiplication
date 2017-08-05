/**
 * Created By Nikhil Gudhka
 * Copyright © Nikhil Gudhka. All rights reserved.
 */

#ifndef SHAREDMEM_H
#define SHAREDMEM_H

#include <unistd.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>

#define FALSE 0
#define TRUE !FALSE

int createShmMem(int M, int N, int K,int** matrix_A,int** matrix_B,int** matrix_C);

typedef struct subtotal{
	int subtotalValue;
	int ID;
}subtotal;

#endif
