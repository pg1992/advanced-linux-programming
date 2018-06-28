#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

pthread_mutex_t mutex;
pthread_mutexattr_t mutex_attr;

void *thread_task(void *arg) {
    long tid = (long) arg;
    int ret, i;
    const int num_locks = 5;

    // Enable asynchronous thread cancellation.
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    fprintf(stderr, "Thread %ld: I started.\n", tid);

    for (i = 1; i <= num_locks; i++) {
        fprintf(stderr, "Thread %ld:"
                        " I will try to get lock number %i.\n", tid, i);
        // We lock NUM_LOCKS times and never unlock.
        ret = pthread_mutex_lock(&mutex);
        if (ret == 0)
            fprintf(stderr, "Thread %ld: I got lock number %i.\n", tid, i);
        else {
            switch(ret) {
                // The error-checking mutex warns about deadlock.
                case EDEADLK:
                    fprintf(stderr, "Thread %ld: %s\n", tid, strerror(ret));
                    break;
                default:
                    fprintf(stderr, "Thread %ld: Unknown error\n", tid);
                    break;
            }
        }
    }

    fprintf(stderr, "Thread %ld: I finished.\n", tid);

    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t tid, other;
    pthread_attr_t attr;
    void *ret;

    // We have three types of mutexes.
    const char *lock_names[] = {"fast\0", "recursive\0", "error-checking\0"};
    int lock_types[] = {
        PTHREAD_MUTEX_NORMAL,
        PTHREAD_MUTEX_RECURSIVE,
        PTHREAD_MUTEX_ERRORCHECK
    };
    int i, sleep_time = 5;

    for (i = 0; i < 3; i++) {
        // Create a mutex attribute with the type specified in LOCK_TYPES.
        pthread_mutexattr_init(&mutex_attr);
        pthread_mutexattr_settype(&mutex_attr, lock_types[i]);
        // Initialize a mutex with that attribute.
        pthread_mutex_init(&mutex, &mutex_attr);
        // We don't need the attribute anymore so we destroy it.
        pthread_mutexattr_destroy(&mutex_attr);

        // Inform which type of mutex we are using.
        printf("Create 2 threads that will lock a %s lock twice.\n", lock_names[i]);
        // Create a thread attribute specifying the thread as joinable.
        // (We don't need to do it since it is the default.)
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
        // Create 2 threads with this attribute and pass their number as
        // arguments.
        pthread_create(&tid, &attr, thread_task, (void *) 1);
        pthread_create(&other, &attr, thread_task, (void *) 2);
        // We don't need the thread attribute anymore so we destroy it.
        pthread_attr_destroy(&attr);

        // Sleep for the amount specified in SLEEP_TIME.
        printf("Sleeping for %ds.\n", sleep_time);
        sleep(sleep_time);
        // Cancel the threads after sleeping because they could be locked.
        pthread_cancel(tid);
        pthread_cancel(other);

        // Destroy the mutex because we'll create it again in the next loop.
        pthread_mutex_destroy(&mutex);

        // Join the threads so we can get the return status and verify if a
        // thread had to be cancelled.
        pthread_join(tid, &ret);
        if (ret == PTHREAD_CANCELED)
            printf("First thread had to be cancelled.\n");
        pthread_join(other, &ret);
        if (ret == PTHREAD_CANCELED)
            printf("Second thread had to be cancelled.\n");

        printf("\n");
    }

    return EXIT_SUCCESS;
}
