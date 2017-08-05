/**
 * Created By Nikhil Gudhka
 * Copyright © Nikhil Gudhka. All rights reserved.
 */
#include <stdio.h>
#include <stdlib.h>
#include "createthread.h"

int main(int argc,char *argv[])
{
	/* Checks if arguments are less then 6 or more then 6, outputs an error for incorrent number of arguments given */
	if(argc<6 || argc > 6)
	{
		printf("ERROR! Too few or Too many arguments passed\n");
	}
	else
	{
		/*
			@param vals - pointer to a struct of type values
			@param matrix_A - a double pointer to a dynamically allocated 2D array of size M*N
			@param matrix_B - a double pointer to a dynamically allocated 2D array of size N*K
			@param matrix_C - a double pointer to a dynamically allocated 2D array of size M*K
			@param file1 - pointer to file 1 given in argument vector
			@param file2 - pointer to file 2 given in argument vector
			@param rowSubtotal - struct of type subtotal
			@param cond1 - variable of type pthread_cond_t
			@param cond2 - variable of type pthread_cond_t
			@param mutex - variable of type pthread_mutex_t
			@param nRead - number of values read
			@param check - FALSE (0) if fscanf performed invalid read, else TRUE
			@param i - loop variable
			@param j - loop variable
			@param M - num of rows in matrix_A and matrix_C
			@param N - num of columns in matrix_A and num of rows in matrix_B
			@param K - num of columns in matrix_B and matrix_C
		 */
        values* vals;
		int **matrix_A, **matrix_B, **matrix_C;
		FILE *file1, *file2;
		subtotal rowSubtotal;
		pthread_cond_t cond1;
		pthread_cond_t cond2;
		pthread_mutex_t mutex;
		int M=0,N=0,K=0,i=0,j=0,condition=0,check=TRUE,nReads=0;

		/*
			initialize values in rowSubtotal to 0
			file1 - pointer to file 1 given in argument vector
			file2 - pointer to file 2 given in argument vector
			M - num of rows in matrix_A and matrix_C
			N - num of columns in matrix_A and num of rows in matrix_B
			K - num of columns in matrix_B and matrix_C
			cond1 - initialize variable of type pthread_cond_t with PTHREAD_COND_INITIALIZER
			cond2 - initialize variable of type pthread_cond_t with PTHREAD_COND_INITIALIZER
			mutex - initialize variable of type pthread_mutex_t with PTHREAD_MUTEX_INITIALIZER
		 */
		rowSubtotal.subtotalValue = 0;
		rowSubtotal.ID = 0;
		file1 = fopen(argv[1],"r");
		file2 = fopen(argv[2],"r");
		M = atoi(argv[3]);
		N = atoi(argv[4]);
		K = atoi(argv[5]);
		cond1 = (pthread_cond_t)PTHREAD_COND_INITIALIZER;
		cond2 = (pthread_cond_t)PTHREAD_COND_INITIALIZER;
		mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;

		/* if file1 or file2 could not be opened print error */
		if((file1==NULL)||(file2==NULL))
		{
			/* if file1 could not be opened print error */
	        if(file1==NULL)
	        {
	            	perror("Error Opening File 1 : ");
	        }
			/* if file2 could not be opened print error */
	       	if(file2==NULL)
	        {
	        		perror("Error Opening File 2 : ");
	        }
	    }
		else
		{
			/* Dynamically allocate memory for vals */
			vals = (values*)malloc(M*sizeof(values));
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
				set value of i to 0
				set value of j to 0
			 */
			i=0,j=0;
			/* keep looping till i < M and no error occured while reading from file */
			do
			{
				/* set value of j to 0 */
				j=0;
				/* keep looping till j < N and no error occured while reading from file */
				do
				{
					/* read values from files and store the value in matrix_A, return the number of items read to nReads */
					nReads=fscanf(file1,"%d",&matrix_A[i][j]);
					/* if the number of values read is not equal to 1 then print an error, else continue reading file */
					if(nReads!=1)
					{
						printf("Error Reading file 1\n");
						check=FALSE;
					}
					/* increment j by 1 */
					j++;
				}
				while((j<N)&&(check==TRUE));
				/* increment i by 1 */
				i++;
			}
			while((i<M)&&(check==TRUE));

			/*
				set value of i to 0
				set value of j to 0
			 */
			i=0,j=0;
			/* keep looping till i < N and no error occured while reading from file */
			do
			{
				/* set value of j to 0 */
				j=0;
				/* keep looping till j < K and no error occured while reading from file */
				do
				{
					/* read values from files and store the value in matrix_B, return the number of items read to nReads */
					nReads=fscanf(file2,"%d ",&matrix_B[i][j]);
					/* if the number of values read is not equal to 1 then print an error, else continue reading file */
					if(nReads!=1)
					{
						printf("Error Reading file 2\n");
						check=FALSE;
					}
					/* increment j by 1 */
					j++;
				}
				while((j<K)&(check==TRUE));
				/* increment i by 1 */
				i++;
			}
			while((i<N)&&(check==TRUE));

			/* if an error occurs while reading file print error */
			if((ferror(file1))||(ferror(file2)))
			{
				if(ferror(file1))
				{
						perror("Error While Reading File 1 : ");
						check=FALSE;
				}
				if(ferror(file2))
				{
						perror("Error While Reading File 2 : ");
						check=FALSE;
				}
			}

			/* if no error while reading execute if */
			if(check==TRUE)
			{
				/* initalize values in struct vals */
	            for(i=0;i<M;i++)
				{
					vals[i].m = i;
					vals[i].M = M;
					vals[i].N = N;
					vals[i].K = K;
					vals[i].matrix_A = matrix_A;
					vals[i].matrix_B = matrix_B;
					vals[i].matrix_C = matrix_C;
					vals[i].rowSubtotal = &rowSubtotal;
					vals[i].cond1 = &cond1;
					vals[i].cond2 = &cond2;
					vals[i].mutex = &mutex;
					vals[i].condition = &condition;
				}
				/*
					createthreads() create required number of threads
				 */
	            createthreads(vals);
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
			/* free matrix_A,matrix_B,matrix_C,vals */
			free(vals);
			free(matrix_A);
			free(matrix_B);
			free(matrix_C);

			/* close file1 and file2 */
			fclose(file1);
			fclose(file2);
        }

    }
	return 0;
}
