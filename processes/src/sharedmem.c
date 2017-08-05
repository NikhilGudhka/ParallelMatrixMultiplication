/**
 * Created By Nikhil Gudhka 
 * Copyright © Nikhil Gudhka. All rights reserved.
 */
#include "sharedmem.h"
/*
    createShmMem() allocate shared memory for matrix_A, matrix_B, matrix_C and subtotal
    @param M - num of rows in matrix_A and matrix_C
    @param N - num of columns in matrix_A and num of rows in matrix_B
    @param K - num of columns in matrix_B and matrix_C
    @param matrix_A - a double pointer to a dynamically allocated 2D array of size M*N
	@param matrix_B - a double pointer to a dynamically allocated 2D array of size N*K
	@param matrix_C - a double pointer to a dynamically allocated 2D array of size M*K
    @return success - returns FALSE (0) if error allocating shared memory, else return TRUE
 */
int createShmMem(int M, int N, int K, int** matrix_A, int** matrix_B, int** matrix_C)
{
    /* @param success - check if any errors occured while allocating shared memory */
    int success=TRUE;
    /*
        @param name_matA - constant shared memory name "matrixA_Shm" for matrix_A
        @param shm_fd_matA - file desriptor for matA
        @param matA - pointer to shared memory block for matrix_A
        @param size_matA - size of matrix_A
     */
    const char* name_matA = "matrixA_Shm";
    int shm_fd_matA;
    int *matA;
    int size_matA = M*N;

    /*
        shm_open() creates/opens POSIX shared memory for matA
        ftruncate() truncates file to specific length
        mmap() create new mapping in the virtual address space of the calling process
     */
    shm_fd_matA = shm_open(name_matA,O_CREAT|O_RDWR,0666);
    ftruncate(shm_fd_matA, size_matA);
    matA = (int*) mmap(0, size_matA, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd_matA, 0);
    /* if creating new mapping in the virtual address space for matA failed print error, set success to FALSE */
    if(matA==MAP_FAILED)
    {
        perror("Error when allocating shared memory : ");
        success=FALSE;
    }
    else
    {
        /*
            @param name_matB - constant shared memory name "matrixB_Shm" for matrix_B
            @param shm_fd_matB - file desriptor for matB
            @param matB - pointer to shared memory block for matrix_B
            @param size_matB - size of matrix_B
         */
        const char* name_matB = "matrixB_Shm";
        int shm_fd_matB;
        int *matB;
        int size_matB = N*K;

        /*
            shm_open() creates/opens POSIX shared memory for matB
            ftruncate() truncates file to specific length
            mmap() create new mapping in the virtual address space of the calling process
         */
        shm_fd_matB = shm_open(name_matB,O_CREAT|O_RDWR,0666);
        ftruncate(shm_fd_matB, size_matB);
        matB = (int*) mmap(0, size_matB, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd_matB, 0);
        /* if creating new mapping in the virtual address space for matB failed print error, set success to FALSE */
        if(matB==MAP_FAILED)
        {
            perror("Error when allocating shared memory : ");
            success=FALSE;
        }
        else
        {
            /*
                @param name_matC - constant shared memory name "matrixC_Shm" for matrix_C
                @param shm_fd_matC - file desriptor for matC
                @param matC - pointer to shared memory block for matrix_C
                @param size_matC - size of matrix_C
             */
            const char* name_matC = "matrixC_Shm";
            int shm_fd_matC;
            int *matC;
            int size_matC = M*K;
            /*
                shm_open() creates/opens POSIX shared memory for matC
                ftruncate() truncates file to specific length
                mmap() create new mapping in the virtual address space of the calling process
             */
            shm_fd_matC = shm_open(name_matC,O_CREAT|O_RDWR,0666);
            ftruncate(shm_fd_matC, size_matC);
            matC = (int*) mmap(0, size_matC, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd_matC, 0);
            /* if creating new mapping in the virtual address space for matC failed print error, set success to FALSE */
            if(matC==MAP_FAILED)
            {
                perror("Error when allocating shared memory : ");
                success=FALSE;
            }
            else
            {
                /*
                    @param name_subtotal - constant shared memory name "subtotal_Shm" for rowSubtotal
                    @param shm_fd_subtotal - file desriptor for rowSubtotal
                    @param rowSubtotal - pointer to shared memory block for subtotal
                    @param size_rowSubtotal - size of rowSubtotal
                 */
                const char* name_subtotal = "subtotal_Shm";
                int shm_fd_subtotal;
                subtotal *rowSubtotal;
                int size_rowSubtotal = 1;

                /*
                    shm_open() creates/opens POSIX shared memory for rowSubtotal
                    ftruncate() truncates file to specific length
                    mmap() create new mapping in the virtual address space of the calling process
                 */
                shm_fd_subtotal = shm_open(name_subtotal,O_CREAT|O_RDWR,0666);
                ftruncate(shm_fd_subtotal, size_rowSubtotal);
                rowSubtotal = (subtotal*) mmap(0, size_rowSubtotal, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd_subtotal, 0);
                /* if creating new mapping in the virtual address space for rowSubtotal failed print error, set success to FALSE */
                if(rowSubtotal==MAP_FAILED)
                {
                    perror("Error when allocating shared memory : ");
                    success=FALSE;
                }
                else
                {
                    /*
                        Declare and initialize int variable
                        @param p - loop variable
                        @param q - loop variable
                        @param count - loop variable, used for populating shared memory
                     */
                    int p=0,q=0,count=0;
                    /* populating matA using values from matrix_A */
                    for(p=0;p<M;p++)
                    {
                        for(q=0;q<N;q++)
                        {
                            matA[count]=matrix_A[p][q];
                            count++;
                        }
                    }
                    count=0;
                    /* populating matB using values from matrix_B */
                    for(p=0;p<N;p++)
                    {
                        for(q=0;q<K;q++)
                        {
                            matB[count]=matrix_B[p][q];
                            count++;
                        }
                    }
                    count=0;
                    /* populating matC using values from matrix_C */
                    for(p=0;p<M;p++)
                    {
                        for(q=0;q<K;q++)
                        {
                            matC[count]=matrix_C[p][q];
                            count++;
                        }
                    }
                    /* initialize struct values subtotalValue and ID to 0 */
                    rowSubtotal->subtotalValue = 0;
                    rowSubtotal->ID = 0;
                }
            }
        }
    }
    return success;
}
