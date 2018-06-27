#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 4

struct job {
    /* Link field for linked list. */
    struct job* next;

    /* Other fields describing work to be done... */
    int factor;
};

/* A linked list of pending jobs. */
struct job* job_queue;
/* An array that stores the result calculated by each thread. */
long accum[NUM_THREADS];
/* A mutex protecting job_queue. */
pthread_mutex_t job_queue_mutex = PTHREAD_MUTEX_INITIALIZER;

/* Process queued jobs until the queue is empty. */
void process_job(struct job* current_job, long tid)
{
    /* Each thread finds the total accumulated of the jobs it processed. */
    accum[tid] += current_job->factor;
}

void* thread_function(void* arg)
{
    /* The main thread assing each thread with a simple thread number. */
    long tid = (long) arg;

    while (1) {
        struct job* next_job;
        /* Get the next available job. */
        pthread_mutex_lock(&job_queue_mutex);
        if (job_queue == NULL)
            next_job = NULL;
        else {
            next_job = job_queue;
            job_queue = job_queue->next;
        }
        pthread_mutex_unlock(&job_queue_mutex);
        if (!next_job) break;
        /* Carry out the work. */
        process_job(next_job, tid);
        /* Clean up. */
        free(next_job);
    }

    return NULL;
}

/**
 * Create a job list with NUM_JOBS jobs.
 */
void init_jobs(int num_jobs)
{
    int i;
    struct job *current_job;

    /* JOB_QUEUE points to the head of the list. */
    job_queue = current_job = (struct job*)malloc(sizeof(struct job));
    current_job->factor = 0;

    for (i = 1; i < num_jobs; i++) {
        /* Create a new node and fill it with some data. */
        current_job->next = (struct job*)malloc(sizeof(struct job));
        current_job = current_job->next;
        current_job->factor = i;
    }
}

int main(int argc, char* argv[])
{
    pthread_t tid[NUM_THREADS];
    long res, i;
    int num_jobs;

    /* Get the number of jobs from the arguments, otherwise fallback
     * to 100 jobs. */
    num_jobs = argc > 1 ? atoi(argv[1]) : 100;

    /* Set time as the new seed. */
    srand(time(NULL));
    /* Create a jobs list with NUM_JOBS jobs. */
    init_jobs(num_jobs);

    /* Set all values in the accumulation array to zero. */
    for (i = 0; i < NUM_THREADS; i++)
        accum[i] = 0;

    /* Create NUM_THREADS and pass its order as the argument. */
    for (i = 0; i < NUM_THREADS; i++)
        pthread_create(tid+i, NULL, thread_function, (void *)i);

    /* Join all threads. */
    for (i = 0; i < NUM_THREADS; i++)
        pthread_join(tid[i], NULL);

    /* Sum all the accumulated values of each thread and prints
     * each individual accumulated result. */
    res = 0;
    for (i = 0; i < NUM_THREADS; i++) {
        printf("Thread %ld accumulated %ld\n", i, accum[i]);
        res += accum[i];
    }

    /* Prints the total accumulated. */
    printf("Total accumulated: %d\n", res);

    return EXIT_SUCCESS;
}
