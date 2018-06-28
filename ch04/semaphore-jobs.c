#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

struct job {
    struct job *next;
    long index;
    long payload;
} *job_queue;

pthread_mutex_t lock_job = PTHREAD_MUTEX_INITIALIZER;
sem_t job_q_count;

void *consumer(void *arg)
{
    long cons_num = (long) arg;
    struct job *this_job;
    struct timespec ts;

    printf("Consumer %ld started.\n", cons_num);

    while (1) {
        clock_gettime(CLOCK_REALTIME, &ts);
        ts.tv_sec += 3;

        if (sem_timedwait(&job_q_count, &ts) < 0) {
            switch(errno) {
                case ETIMEDOUT:
                    printf("Consumer %ld waited more than 3s for data.\n",
                            cons_num);
                    return NULL;
                    break;
                default:
                    printf("Consumer %ld received a bizarre error: %s.\n",
                            cons_num, strerror(errno));
                    return NULL;
                    break;
            }
        }

        pthread_mutex_lock(&lock_job);
        this_job = job_queue;
        job_queue = job_queue->next;
        pthread_mutex_unlock(&lock_job);

        free(this_job);

        printf("Consumer %ld got job %ld with payload %ld.\n",
                cons_num, this_job->index, this_job->payload);
    }
    printf("Consumer %ld finished.\n", cons_num);

    return NULL;
}

void *producer(void *arg)
{
    long prod_num = (long) arg;
    struct job *new_job;
    long jobs = 20;

    printf("Producer %ld started.\n", prod_num);

    while (jobs-- > 0) {
        new_job = (struct job *)malloc(sizeof(struct job));

        pthread_mutex_lock(&lock_job);

        new_job->next = job_queue;
        new_job->index = job_queue ? job_queue->index + 1 : 0;
        new_job->payload = rand();
        job_queue = new_job;
        sem_post(&job_q_count);

        pthread_mutex_unlock(&lock_job);

        printf("Producer %ld created job %ld with payload %ld.\n",
                prod_num, new_job->index, new_job->payload);
    }

    printf("Producer %ld finished.\n", prod_num);

    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t *prod_tid;
    pthread_t *cons_tid;
    long total_consumers = 4;
    long total_producers = 4;
    long i;

    prod_tid = (pthread_t *)malloc(total_producers * sizeof(pthread_t));
    cons_tid = (pthread_t *)malloc(total_consumers * sizeof(pthread_t));

    sem_init(&job_q_count, 0, 0);

    for (i = 0; i < total_producers; i++)
        pthread_create(&prod_tid[i], NULL, producer, (void *) i + 1);

    sleep(2);

    for (i = 0; i < total_consumers; i++)
        pthread_create(&cons_tid[i], NULL, consumer, (void *) i + 1);

    for (i = 0; i < total_producers; i++)
        pthread_join(prod_tid[i], NULL);

    for (i = 0; i < total_consumers; i++)
        pthread_join(cons_tid[i], NULL);

    free(prod_tid);
    free(cons_tid);

    return EXIT_SUCCESS;
}
