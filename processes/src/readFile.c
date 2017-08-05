/**
 * Created By Nikhil Gudhka 
 * Copyright © Nikhil Gudhka. All rights reserved.
 */
#include "readFile.h"
/*
	readFile() reads the files and initializes the arrays created in main()
	@param matrix_A - a double pointer to a dynamically allocated 2D array of size M*N
	@param matrix_B - a double pointer to a dynamically allocated 2D array of size N*K
	@param matrix_C - a double pointer to a dynamically allocated 2D array of size M*K
	@param argv - argument vector, 2D array of type char holding the arguments given to the program
	@return success - returns FALSE (0) if error reading file, else return TRUE
 */
int readFile(int** matrix_A, int** matrix_B, int** matrix_C,char* argv[])
{
	/*
		@param file1 - pointer to file 1 given in argument vector
		@param file2 - pointer to file 2 given in argument vector
		@param success - check if any errors occured while reading file
		@param nRead - number of values read
		@param check - FALSE (0) if fscanf performed invalid read, else TRUE
		@param i - loop variable
		@param j - loop variable
		@param M - num of rows in matrix_A and matrix_C
		@param N - num of columns in matrix_A and num of rows in matrix_B
		@param K - num of columns in matrix_B and matrix_C
	 */
	FILE *file1,*file2;
	int success=FALSE,nReads=0,check=TRUE,i=0,j=0,M=0,N=0,K=0;
	file1 = fopen(argv[1],"r");
	file2 = fopen(argv[2],"r");
	M = atoi(argv[3]);
	N = atoi(argv[4]);
	K = atoi(argv[5]);
	/* if file1 or file2 could not be opened print error */
	if((file1==NULL)||(file2==NULL))
	{
		/* if file1 could not be opened print error */
        if(file1==NULL)
        {
            	perror("Error Opening File 1 : ");
				success=FALSE;
        }
		/* if file2 could not be opened print error */
       	if(file2==NULL)
        {
        		perror("Error Opening File 2 : ");
				success=FALSE;
        }
    }
	else
	{
		/*
			set success to TRUE
			set value of i to 0
			set value of j to 0
		 */
		success=TRUE;
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
					success=FALSE;
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
					success=FALSE;
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
	        }
	       	if(ferror(file2))
	        {
	        		perror("Error While Reading File 2 : ");
	        }
	    }
		/* close file1 and file2 */
		fclose(file1);
		fclose(file2);
	}

	return success;
}
