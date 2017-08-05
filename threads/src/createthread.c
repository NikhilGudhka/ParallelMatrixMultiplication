/**
 * Created By Nikhil Gudhka
 * Copyright © Nikhil Gudhka. All rights reserved.
 */
#include "createthread.h"
/*
    createthreads() creates threads to calculate values and subtotal
 */
void createthreads(values* vals)
{
    /*
        @param error - check if error occured creating thread
        @param tid - pointer to type pthread_t
        @param attr - attributes of new thread
        @param i - loop variable
        @param total - store sum of subtotal
     */
    int error=FALSE;
    pthread_t *tid;
    pthread_attr_t attr;
    int i=0,total=0;;

    /* Dynamically allocate memory for tid */
    tid = (pthread_t*)malloc(vals->M*sizeof(pthread_t));
    /* initialize thread attriutes */
    pthread_attr_init(&attr);

    /* loop to create M number of threads if no error occured creating previous thread */
    while((i<vals->M)&&(error==0))
    {
        /* pthread_create() create thread with ID tid, attriutes attr, execute function matrixmult() with arguments vals */
        error=pthread_create(&tid[i],&attr,matrixmult,&vals[i]);
        /* if error while creating thread print error */
        if(error!=0)
        {
            printf("Error creating child\n");
            error=TRUE;
        }
        /* increment i by 1 */
        i++;
    }
    /* if no error occured creating threads run if */
    if(error==FALSE)
    {
        /* loop M number of times */
        for(i=0;i<vals->M;i++)
        {
            /*
                set mutex lock on vals->mutex
                keep waiting until vals->condition != 0
                store sum of subtotals in variable total
                set vals->condition to 0
                pthread_cond_signal on vals->cond1 to signal threads created to calculate row subtotal to continue execution
                unlock mutex vals->mutex
             */
            pthread_mutex_lock (vals->mutex);
            while(*(vals->condition)==0)
            {
                pthread_cond_wait (vals->cond2,vals->mutex);
            }
            printf("Subtotal produced by thread with ID %u: %d\n",vals[i].rowSubtotal->ID,vals[i].rowSubtotal->subtotalValue);
            total+=vals[i].rowSubtotal->subtotalValue;
            *(vals->condition)=0;
            pthread_cond_signal(vals->cond1);
            pthread_mutex_unlock (vals->mutex);
        }
        printf("Total: %d\n",total);
    }
    /*
        joining threads to main thread
        closes the thread with id tid[i]
     */
    for(i=0;i<vals->M;i++)
    {
        pthread_join(tid[i],NULL);
        pthread_cancel(tid[i]);
    }
    /* free tid */
    free(tid);
}
