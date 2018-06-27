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
    int ret;

    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    fprintf(stderr, "Thread %ld started.\n", tid);
    fprintf(stderr, "Thread %ld will try to get first lock.\n", tid);
    ret = pthread_mutex_lock(&mutex);
    if (ret == 0)
        fprintf(stderr, "Thread %ld got first lock.\n", tid);
    else {
        switch(ret) {
            case EDEADLK:
                fprintf(stderr, "Thread %ld: %s\n", tid, strerror(ret));
                break;
            default:
                break;
        }
    }
    fprintf(stderr, "Thread %ld will try to get second lock.\n", tid);
    ret = pthread_mutex_lock(&mutex);
    if (ret == 0)
        fprintf(stderr, "Thread %ld got second lock.\n", tid);
    else {
        switch(ret) {
            case EDEADLK:
                fprintf(stderr, "Thread %ld: %s\n", tid, strerror(ret));
                break;
            default:
                break;
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

    pthread_mutexattr_init(&mutex_attr);
    pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_NORMAL);
    pthread_mutex_init(&mutex, &mutex_attr);
    pthread_mutexattr_destroy(&mutex_attr);

    printf("Creating 2 threads that will lock a fast lock twice.\n");
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

    pthread_mutexattr_init(&mutex_attr);
    pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&mutex, &mutex_attr);
    pthread_mutexattr_destroy(&mutex_attr);

    printf("\nCreating 2 threads that will lock a recursive lock twice.\n");
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

    pthread_mutexattr_init(&mutex_attr);
    pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_ERRORCHECK);
    pthread_mutex_init(&mutex, &mutex_attr);
    pthread_mutexattr_destroy(&mutex_attr);

    printf("\nCreating 2 threads that will lock a error-checking lock twice.\n");
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

    return EXIT_SUCCESS;
}
