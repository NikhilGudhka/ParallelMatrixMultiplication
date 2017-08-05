/**
 * Created By Nikhil Gudhka
 * Copyright © Nikhil Gudhka. All rights reserved.
 */
#include "matrixMult.h"
/*
    matrixmult() calculate the matrix multiplication values from the matrix row passed
 */
void *matrixmult(void* val1)
{
    /*
        typecast val1 to type pointer to values and store in vals
        @param k - loop variable for number of columns
        @param n - loop variable for number of rows
     */
    values* vals=(values*)val1;
    int k=0,n=0,temptotal=0;
    /* calculate the matrix multiplication values from the matrix row passed, store subtotal in temptotal temporarily */
    for(k=0;k<vals->K;k++)
    {
        for(n=0;n<vals->N;n++)
        {
            vals->matrix_C[vals->m][k]+=vals->matrix_A[vals->m][n]*vals->matrix_B[n][k];
        }
        temptotal+=vals->matrix_C[vals->m][k];
    }

    /*
        set mutex lock on vals->mutex
        keep waiting until vals->condition != 1
        set vals->condition to 1
        stores value of temptotal in struct rowSubtotal in struct vals
        stores value of thread ID in struct rowSubtotal in struct vals
        pthread_cond_signal on vals->cond2 to signal main thread to continue execution
        unlock mutex vals->mutex
     */
    pthread_mutex_lock (vals->mutex);
    while(*(vals->condition)==1)
    {
        pthread_cond_wait (vals->cond1,vals->mutex);
    }
    *(vals->condition)=1;
    vals->rowSubtotal->subtotalValue=temptotal;
    vals->rowSubtotal->ID=(unsigned int)pthread_self();
    pthread_cond_signal(vals->cond2);
    pthread_mutex_unlock (vals->mutex);
    /* exit thread */
    //pthread_exit(NULL);
    return NULL; 
}
