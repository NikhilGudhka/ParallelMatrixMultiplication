/**
 * Created By Nikhil Gudhka
 * Copyright © Nikhil Gudhka. All rights reserved.
 */
#ifndef CREATETHREAD_H
#define CREATETHREAD_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "matrixMult.h"

#define FALSE 0
#define TRUE !FALSE

typedef struct subtotal{
	int subtotalValue;
	int ID;
}subtotal;

typedef struct values{
    int m;
    int M;
    int K;
    int N;
    int** matrix_A;
    int** matrix_B;
    int** matrix_C;
	subtotal* rowSubtotal;
	pthread_mutex_t* mutex;
	pthread_cond_t* cond1;
	pthread_cond_t* cond2;
	int* condition;
}values;

void createthreads(values* vals);

#endif
