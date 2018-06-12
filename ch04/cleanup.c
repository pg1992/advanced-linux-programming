#include <malloc.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

/* Allocate a temporary buffer. */
void* allocate_buffer(size_t size)
{
    printf("Allocating buffer.\n");
    return malloc(size);
}

/* Deallocate a temporary buffer. */
void deallocate_buffer(void* buffer)
{
    printf("Deallocating buffer (cleanup handler).\n");
    free(buffer);
}

void* do_some_work(void* args)
{
    printf("Starting thread.\n");
    /* Allocate a temporary buffer. */
    void* temp_buffer = allocate_buffer(1024);
    printf("Pushing cleanup handler.\n");
    /* Register a cleanup handler for this buffer, to deallocate it in
     * case the thread exits or is cancelled. */
    pthread_cleanup_push(deallocate_buffer, temp_buffer);
    printf("Sleeping for 5 s.\n");
    sleep(5);
    printf("Popping the cleanup handler.\n");
    /* Unregister the cleanup handler.  Because we a pass a nonzero value,
     * this actually performs the cleanup by calling
     * deallocate_buffer. */
    pthread_cleanup_pop(1);
}

int main(void)
{
    /* Our thread ID. */
    void* res;
    pthread_t thread;

    printf("Starting thread.\n");
    /* Create a new thread. */
    pthread_create(&thread, NULL, do_some_work, NULL);
    printf("Sleeping for 1 s.\n");
    /* Wait 1 s before canceling the thread. */
    sleep(1);
    printf("Cancelling thread.\n");
    /* Cancel the thread which will effectively call the cleanup handler. */
    pthread_cancel(thread);
    /* The only way to know if a thread was canceled is by joining it.. */
    pthread_join(thread, &res);

    /* Check if the thread was correctly canceled. */
    if (res == PTHREAD_CANCELED)
        printf("Thread was canceled.\n");
    else
        printf("Thread wasn't canceled");

    return 0;
}
