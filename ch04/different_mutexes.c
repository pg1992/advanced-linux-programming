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

    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    fprintf(stderr, "Thread %ld started.\n", tid);

    for (i = 1; i <= num_locks; i++) {
        fprintf(stderr, "Thread %ld:"
                        " I will try to get lock number %i.\n", tid, i);
        ret = pthread_mutex_lock(&mutex);
        if (ret == 0)
            fprintf(stderr, "Thread %ld: I got lock number %i.\n", tid, i);
        else {
            switch(ret) {
                case EDEADLK:
                    fprintf(stderr, "Thread %ld: %s\n", tid, strerror(ret));
                    break;
                default:
                    break;
            }
        }
    }

    fprintf(stderr, "Thread %ld finished.\n", tid);

    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t tid, other;
    pthread_attr_t attr;
    void *ret;
    const char *lock_names[] = {"fast\0", "recursive\0", "error-checking\0"};
    int lock_types[] = {
        PTHREAD_MUTEX_NORMAL,
        PTHREAD_MUTEX_RECURSIVE,
        PTHREAD_MUTEX_ERRORCHECK
    };
    int i;

    for (i = 0; i < 3; i++) {
        pthread_mutexattr_init(&mutex_attr);
        pthread_mutexattr_settype(&mutex_attr, lock_types[i]);
        pthread_mutex_init(&mutex, &mutex_attr);
        pthread_mutexattr_destroy(&mutex_attr);

        printf("Creating 2 threads that will lock a %s lock twice.\n", lock_names[i]);
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
        pthread_create(&tid, &attr, thread_task, (void *) 1);
        pthread_create(&other, &attr, thread_task, (void *) 2);
        pthread_attr_destroy(&attr);

        printf("Sleeping for 5s.\n");
        sleep(5);
        pthread_cancel(tid);
        pthread_cancel(other);

        pthread_mutex_destroy(&mutex);
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
