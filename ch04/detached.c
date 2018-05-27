#include <unistd.h>
#include <pthread.h>
#include <stdio.h>

void* thread_function(void* arg)
{
    printf("I'm a detached thread!\n");
}

int main(void)
{
    pthread_attr_t attr;
    pthread_t thread_id;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    /* No need to join this thread. */
    pthread_create(&thread_id, &attr, &thread_function, NULL);
    pthread_attr_destroy(&attr);

    printf("Sleeping in the main thread for a bit...\n");
    sleep(1);

    return 0;
}
