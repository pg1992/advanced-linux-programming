#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>

static pthread_t main_thread;

static void handler(int signum)
{
    pthread_t caller_id = pthread_self();
    /* Prints the ID of the thread which received SIGINT (^C). */
    fprintf(stderr,"thread %li received SIGINT\n", (long) caller_id);
    if (pthread_equal(caller_id, main_thread)) {
        fprintf(stderr,"main thread closing the whole thing\n");
        fprintf(stderr,"\t\t\t\t\tthe motherf&$@ing user killed me :(\n");
        fprintf(stderr,"\t\t\t\t\tbut i'll bring everyone down with me >:D\n");
        exit(0);
    }
    fprintf(stderr,"\t\t\t\t\tyou shot me down (bang, bang)\n");
    fprintf(stderr,"\t\t\t\t\ti hit the ground (bang, bang)\n");
    pthread_exit(0);
}

void *thread_function(void *arg)
{
    pthread_t other = (pthread_t) arg;
    if (other) {
        fprintf(stderr,"the other thread ID is %ld\n", (long) other);
        fprintf(stderr,"\t\t\t\t\ti'll kill it in 5 secs\n");
        sleep(5);
        fprintf(stderr,"\t\t\t\t\tI'LL KILL YOUUUU!!!\n");
        pthread_kill(other, SIGINT);
    }
    /* Announce that the thread has begun and show its ID. */
    fprintf(stderr,"entered on thread %li\n", (long) pthread_self());

    /* Loop forever... */
    while(1) {
        fprintf(stderr, "heartbeat from %ld\n", (long) pthread_self());
        sleep(1);
    }

    return NULL;
}

int main()
{
    main_thread = pthread_self();
    /* We will use two threads. */
    pthread_t tid[2];

    /* Create the two threads. */
    pthread_create(tid, NULL, thread_function, NULL);
    pthread_create(tid+1, NULL, thread_function, (void *) tid[0]);

    /* Print the ID of each thread. */
    fprintf(stderr,"first thread ID is %li\n", (long) tid[0]);
    fprintf(stderr,"second thread ID is %li\n", (long) tid[1]);

    /* Signal handling struct. */
    struct sigaction sa;

    /* Use the function `handler` as the handler. */
    sa.sa_handler = handler;
    /* Doesn't block other signals. */
    sigemptyset(&sa.sa_mask);
    /* Restart interruptible functions. */
    sa.sa_flags = SA_RESTART;

    /* Announce the ID of the main thread. */
    fprintf(stderr,"main thread ID is %li\n", (long) pthread_self());

    /* Register the signal metadata to SIGINT. */
    if(sigaction(SIGINT, &sa, (void *) 0) == -1)
        return -1;

    /* Loop forever... */
    while(1);

    return 0;
}
