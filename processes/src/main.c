/**
 * Created By Nikhil Gudhka 
 * Copyright © Nikhil Gudhka. All rights reserved.
 */
#include <stdio.h>
#include <stdlib.h>
#include "matrixMult.h"
#include "sharedmem.h"
#include "readFile.h"

int main(int argc,char *argv[])
{
	/* Checks if arguments are less then 6 or more then 6, outputs an error for incorrect number of arguments given */
	if(argc<6 || argc > 6)
	{
		printf("ERROR! Too few or Too many arguments passed\n");
	}
	else
	{
		/*
			Declare 3 variables of type double pointer to int
			@param matrix_A - a double pointer to a dynamically allocated 2D array of size M*N
			@param matrix_B - a double pointer to a dynamically allocated 2D array of size N*K
			@param matrix_C - a double pointer to a dynamically allocated 2D array of size M*K
		 */
		int **matrix_A, **matrix_B, **matrix_C;
		/*
			Declare and initialize int variables
			@param M - num of rows in matrix_A and matrix_C
			@param N - num of columns in matrix_A and num of rows in matrix_B
			@param K - num of columns in matrix_B and matrix_C
			@param i - loop variable
			@param successRead - check if any errors occured while reading file
			@param successMem - check if any errors occured while allocating shared memory
		 */
		int M=0,N=0,K=0,i=0,successRead=FALSE,successMem=FALSE;
		/*
			Function atoi used to convert the char* arguments to int.
		 */
		M = atoi(argv[3]);
		N = atoi(argv[4]);
		K = atoi(argv[5]);
		/*
			Dynamically allocate memory for matrix_A, allocate memory for 1D array first then run for loop to allocate memory to create 2D array.
			Initialize all values in the 2D array (matrix_A) to 0 using calloc
		 */
		matrix_A = (int**)calloc(M,sizeof(int*));
		for(i=0;i<M;i++)
		{
			matrix_A[i] = (int*)calloc(N,sizeof(int));
		}
		/*
			Dynamically allocate memory for matrix_B, allocate memory for 1D array first then run for loop to allocate memory to create 2D array.
			Initialize all values in the 2D array (matrix_B) to 0 using calloc
		 */
		matrix_B = (int**)calloc(N,sizeof(int*));
		for(i=0;i<N;i++)
		{
			matrix_B[i] = (int*)calloc(K,sizeof(int));
		}
		/*
			Dynamically allocate memory for matrix_C, allocate memory for 1D array first then run for loop to allocate memory to create 2D array.
			Initialize all values in the 2D array (matrix_C) to 0 using calloc
		 */
		matrix_C = (int**)calloc(M,sizeof(int*));
		for(i=0;i<M;i++)
		{
			matrix_C[i] = (int*)calloc(K,sizeof(int));
		}
		/*
			readFile() takes a pointer to matrix_A, matrix_B, matrix_C, and pointer to argument vector argv
			@param matrix_A - a double pointer to a dynamically allocated 2D array of size M*N
			@param matrix_B - a double pointer to a dynamically allocated 2D array of size N*K
			@param matrix_C - a double pointer to a dynamically allocated 2D array of size M*K
			@param argv - argument vector, 2D array of type char holding the arguments given to the program
			@return successRead - returns FALSE (0) if error reading file, else return TRUE
		 */
		successRead=readFile(matrix_A,matrix_B,matrix_C,argv);
		/*
			check if successRead is TRUE, execute code inside if statement only if TRUE
		 */
		if(successRead==TRUE)
		{
			/*
				createShmMem() allocates shared memory
				@param M - num of rows in matrix_A and matrix_C
				@param N - num of columns in matrix_A and num of rows in matrix_B
				@param K - num of columns in matrix_B and matrix_C
				@param matrix_A - a double pointer to a dynamically allocated 2D array of size M*N
				@param matrix_B - a double pointer to a dynamically allocated 2D array of size N*K
				@param matrix_C - a double pointer to a dynamically allocated 2D array of size M*K
				@return successMem - returns FALSE (0) if error allocating shared memory, else return TRUE
			 */
			successMem=createShmMem(M,N,K,matrix_A,matrix_B,matrix_C);
			/*
				check if successMem is TRUE, execute code inside if statement only if TRUE
			 */
			if(successMem==TRUE)
			{
				/*
					matrixMult() fork required number of children, and perform the parallel matrix multiplication sum
					@param M - num of rows in matrix_A and matrix_C
					@param N - num of columns in matrix_A and num of rows in matrix_B
					@param K - num of columns in matrix_B and matrix_C
				 */
				matrixMult(M,N,K);
			}
		}
		/* loop and free the 2D arrays matrix_A and matrix_C */
		for(i=0;i<M;i++)
		{
			free(matrix_A[i]);
			free(matrix_C[i]);
		}
		/* loop and free the 2D array matrix_B */
		for(i=0;i<N;i++)
		{
			free(matrix_B[i]);
		}
		/* free matrix_A,matrix_B,matrix_C */
		free(matrix_A);
		free(matrix_B);
		free(matrix_C);
    }
	return 0;
}
