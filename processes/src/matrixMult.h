/**
 * Created By Nikhil Gudhka 
 * Copyright © Nikhil Gudhka. All rights reserved.
 */
#ifndef MATRIXMULT_H
#define MATRIXMULT_H

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <semaphore.h>
#include "sharedmem.h"

void matrixMult(int M, int N, int K);

#endif
