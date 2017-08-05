/**
 * Created By Nikhil Gudhka
 * Copyright © Nikhil Gudhka. All rights reserved.
 */
#include "matrixMult.h"
/*
    matrixMult() fork required number of children, and perform the parallel matrix multiplication sum
    @param M - num of rows in matrix_A and matrix_C
    @param N - num of columns in matrix_A and num of rows in matrix_B
    @param K - num of columns in matrix_B and matrix_C
 */
void matrixMult(int M, int N, int K)
{
    /*
        @param success - check if any errors occured while forking
        @param sem - creates new POSIX semaphore
        @param sem2 - creates new POSIX semaphore
     */
    int success=TRUE;
    sem_t *sem = sem_open("key",O_CREAT,0666,0);
    sem_t *sem2 = sem_open("key2",O_CREAT,0666,1);

    /*
        @param parentID - to store the parent process ID
        @param m -loop variable for number of rows in matrix_A and matrix_C
        @param processType - check if process is parent or child
     */
    int parentID=getpid();
    int m=0,processType=1;
    /*
        create M number of childs if processType is parent (1), and no error occured creating previous child
     */
    while((m<M)&&(processType==1)&&(success==TRUE))
    {
        /* fork() create new child */
        pid_t pid=fork();
        /* fork() success if pid = 0 */
        if (pid==0)
        {
            /*
                @param k - loop variable
                @param n - loop variable
                @param count1 - loop variable for shared memory
                @param count2 - loop variable for shared memory
                @param temptotal - hold subtotal temporarily
                @param matA - pointer to shared memory block for matrix_A
                @param matB - pointer to shared memory block for matrix_B
                @param matC - pointer to shared memory block for matrix_C
                @param rowSubtotal - pointer to shared memory block for subtotal
             */
            int k=0,n=0,count=0,count1=0,count2=0,temptotal=0;
            int *matA, *matB, *matC;
            subtotal *rowSubtotal;

            /*
                @param size_matA - size of matrix_A
                @param size_matB - size of matrix_B
                @param size_matC - size of matrix_C
                @param size_rowSubtotal - size of subtotal
             */
            int size_matA = M*N;
            int size_matB = N*K;
            int size_matC = M*K;
            int size_rowSubtotal = 1;

            /*
                @param shm_fd_matA - opening shared memory block "matrixA_Shm" for reading
                @param shm_fd_matB - opening shared memory block "matrixB_Shm" for reading
                @param shm_fd_matC - opening shared memory block "matrixC_Shm" for reading and writing
                @param shm_fd_subtotal - opening shared memory block "subtotal_Shm" for reading and writing
             */
            int shm_fd_matA=shm_open("matrixA_Shm", O_RDONLY, 0666);
            int shm_fd_matB=shm_open("matrixB_Shm", O_RDONLY, 0666);
            int shm_fd_matC=shm_open("matrixC_Shm", O_RDWR, 0666);
            int shm_fd_subtotal=shm_open("subtotal_Shm", O_RDWR, 0666);

            /*
                matA pointer to shared memory block for matrix_A
                matB pointer to shared memory block for matrix_B
                matC pointer to shared memory block for matrix_C
                rowSubtotal pointer to shared memory block for subtotal
             */
            matA = (int*) mmap(0, size_matA, PROT_READ, MAP_SHARED, shm_fd_matA, 0);
            matB = (int*) mmap(0, size_matB, PROT_READ, MAP_SHARED, shm_fd_matB, 0);
            matC = (int*) mmap(0, size_matC, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd_matC, 0);
            rowSubtotal = (subtotal*) mmap(0, size_rowSubtotal, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd_subtotal, 0);

            /*
                arrays stored in shared memory are 1D, so calcuating index depending on bytes
                if the row is 0 start mutliplication from index 0
                else if row is not 0 calculate number of bytes to the value of row m at index of that value, then start matrix multiplication
             */
            if(m==0)
            {
                /*
                    K - number of coulumns in matrix_B
                    count2 - row index for matrix_B
                    count2+=K - moves pointer ahead by K to point to next row index
                    count - index of matrix_C
                    temptotal - store subtotal temporarily
                 */
                for(k=0;k<K;k++)
                {
                    count2=k;
                    for(n=0;n<N;n++)
                    {
                        matC[count]+=matA[n]*matB[count2];
                        count2+=K;
                    }
                    temptotal+=matC[count];
                    count++;
                }
            }
            else
            {
                /*
                    count - index of matrix_C
                    count1 - column index for matrix_A
                    count2 - row index for matrix_B
                    count2+=K - moves pointer ahead by K to point to next row index
                    temptotal -

                    count - calculate the index to start putting values in matrixc
                    count 1 = give index of colum for matrix A row m
                    count 2 = give index of row for ,atrix B for column k
                    temptotal - store subtotal temporarily
                 */
                count=m*K;
                count1=m*N;
                count2=0;
                for(k=0;k<K;k++)
                {
                    count2=k;
                    for(n=0;n<N;n++)
                    {
                        matC[count]+=matA[count1]*matB[count2];
                        count1++;
                        count2+=K;
                    }
                    temptotal+=matC[count];
                    count++;
                    count1=m*N;
                }

            }

            /*
                sem_wait() wait on semaphore sem2, reduces sem2 value by 1
                stores value of temptotal in struct rowSubtotal
                stores value of child process ID in struct rowSubtotal
                sem_post() increment sem value by 1
                processType - indicates process is child (0)
                _exit() to exit child process imediately once child had completed
             */
            sem_wait(sem2);
            rowSubtotal->subtotalValue=temptotal;
            rowSubtotal->ID = getpid();
            fflush(stdout);                                            
            sem_post(sem);
            processType=0;
            _exit(0);
        }
        /* print error if fork() failed */
        else if(pid==-1)
        {
            printf("Error forking process\n");
            success=FALSE;
        }
        /* set processType to 1 to indicate parent process */
        else
        {
            processType=1;
        }
        /* Incement m by 1 */
        m++;
    }

    if(getpid()==parentID)
    {
        /*
            @param i - loop variable
            @param total - hold total
            @param rowSubtotal - pointer to shared memory block for subtotal
         */
        int i=0,total=0;
        subtotal *rowSubtotal;

        /*
            @param size_matA - size of matrix_A
            @param size_matB - size of matrix_B
            @param size_matC - size of matrix_C
            @param size_rowSubtotal - size of subtotal
         */
        int size_matA = M*N;
        int size_matB = N*K;
        int size_matC = M*K;
        int size_subtot = 1;

        /* @param shm_fd_subtotal - opening shared memory block "subtotal_Shm" for reading and writing */
        int shm_fd_subtotal=shm_open("subtotal_Shm", O_RDWR, 0666);

        /* rowSubtotal pointer to shared memory block for subtotal */
        rowSubtotal = (subtotal*) mmap(0, size_subtot, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd_subtotal, 0);

        /* If no error while forking children run if */
        if(success==TRUE)
        {
            /*
                loop each child
                sem_wait() wait on semaphore sem, reduces sem value by 1
                store sum of subtotals in variable total
                sem_post() increment sem2 value by 1
             */
            for(i=0;i<M;i++)
            {
                sem_wait(sem);
                printf("Subtotal produced by process with ID %d: %d\n",rowSubtotal->ID,rowSubtotal->subtotalValue);
                total+=rowSubtotal->subtotalValue;
                fflush(stdout);
                sem_post(sem2);
            }
            printf("Total: %d\n",total);
        }
        /* Memory unmap all shared memory */
        munmap("matrixA_Shm",size_matA);
        munmap("matrixB_Shm",size_matB);
        munmap("matrixC_Shm",size_matC);
        munmap("subtotal_Shm",size_subtot);
    }
    /* close the semaphores sem and sem2 */
    sem_close(sem);
    sem_close(sem2);

}
