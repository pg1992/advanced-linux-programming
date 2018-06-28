#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void* task(void *arg)
{
    int ret;
    long thread_num = (long) arg;
    char preamble[32];

    sprintf(preamble, "thread %ld: ", thread_num);

    while (ret = pthread_mutex_trylock(&lock)) {
        switch (ret) {
            case EBUSY:
                fprintf(stderr, "%s lock is in use.\n", preamble);
                break;
            case EDEADLK:
                fprintf(stderr, "%s deadlock.\n", preamble);
                break;
            default:
                fprintf(stderr, "%s bizarre error.\n", preamble);
                break;
        }
        usleep(50000);
    }

    fprintf(stderr, "%s lock acquired.  Sleeping a little.\n", preamble);
    sleep(1);

    pthread_mutex_unlock(&lock);

    return NULL;
}

int main(int argc, char *argv[])
{
    int numthreads = 4;
    long i;

    if (argc > 1)
        numthreads = atoi(argv[1]);

    pthread_t tid[numthreads];

    for (i = 0; i < numthreads; i++)
        pthread_create(&tid[i], NULL, task, (void *) i+1);

    for (i = 0; i < numthreads; i++)
        pthread_join(tid[i], NULL);

    pthread_mutex_destroy(&lock);

    return 0;
}
