#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>

static void handler(int signum)
{
    /* Prints the ID of the thread which received SIGINT (^C). */
    printf("thread %li received SIGINT\n", (long) pthread_self());
    exit(0);
}

void *thread_function(void *arg)
{
    /* Announce that the thread has begun and show its ID. */
    printf("entered on thread %li\n", (long) pthread_self());

    /* Loop forever... */
    while(1);

    return NULL;
}

int main()
{
    /* We will use two threads. */
    pthread_t tid[2];

    /* Create the two threads. */
    pthread_create(tid, NULL, thread_function, NULL);
    pthread_create(tid+1, NULL, thread_function, NULL);

    /* Print the ID of each thread. */
    printf("first thread ID is %li\n", (long) tid[0]);
    printf("second thread ID is %li\n", (long) tid[1]);

    /* Signal handling struct. */
    struct sigaction sa;

    /* Use the function `handler` as the handler. */
    sa.sa_handler = handler;
    /* Doesn't block other signals. */
    sigemptyset(&sa.sa_mask);
    /* Restart interruptible functions. */
    sa.sa_flags = SA_RESTART;

    /* Announce the ID of the main thread. */
    printf("main thread ID is %li\n", (long) pthread_self());

    /* Register the signal metadata to SIGINT. */
    if(sigaction(SIGINT, &sa, (void *) 0) == -1)
        return -1;

    /* Loop forever... */
    while(1);

    return 0;
}
