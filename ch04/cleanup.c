#include <malloc.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

void* allocate_buffer(size_t size)
{
    printf("Allocating buffer.\n");
    return malloc(size);
}

void deallocate_buffer(void* buffer)
{
    printf("Deallocating buffer (cleanup handler).\n");
    free(buffer);
}

void* do_some_work(void* args)
{
    printf("Starting thread.\n");
    void* temp_buffer = allocate_buffer(1024);
    printf("Pushing cleanup handler.\n");
    pthread_cleanup_push(deallocate_buffer, temp_buffer);
    printf("Sleeping for 5 s.\n");
    sleep(5);
    printf("Popping the cleanup handler.\n");
    pthread_cleanup_pop(1);
}

int main(void)
{
    pthread_t thread;

    printf("Starting thread.\n");
    pthread_create(&thread, NULL, do_some_work, NULL);
    printf("Sleeping for 1 s.\n");
    sleep(1);
    printf("Cancelling thread.\n");
    pthread_cancel(thread);
    pthread_join(thread, NULL);

    return 0;
}
