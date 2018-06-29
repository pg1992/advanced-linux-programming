#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

// The structure containing each job description.
struct job {
    struct job *next; // Pointer to the next job.
    long index;       // Index of the job.
    long payload;     // Payload to be processed.
} *job_queue;

// Mutex for protecting job access for both producers and consumers.
static pthread_mutex_t lock_job = PTHREAD_MUTEX_INITIALIZER;
// Semaphore indicating that a job was added.
static sem_t job_q_count;
// This is just a counter that the producer uses to update each job index.
static long job_counter = 0;

/*
 * Consume the data in job queue in a loop and only start consuming if
 * there is data to consume (sem_trywait returns).  The task ends if more
 * than 3s is passed.
 */
void *consumer(void *arg)
{
    // The thread sequence number passed on creation.
    long cons_num = (long) arg;
    // Local pointer to this thread job.
    struct job *this_job;
    // This variable holds the final wait time of the semaphore.
    struct timespec ts;

    // A prompt to indentify that thread CONS_NUM has started.
    printf("Consumer %ld started.\n", cons_num);

    // Loop forever.
    while (1) {
        // Assign the wall-clock time to TS.
        clock_gettime(CLOCK_REALTIME, &ts);
        // TS will be 3s in the future which is the time of timeout.
        ts.tv_sec += 3;

        // Wait until there's a job to be processed or 3s has passed.
        if (sem_timedwait(&job_q_count, &ts) < 0) {
            // An error occurred!  Is it a timeout error on something
            // different?
            switch(errno) {
                case ETIMEDOUT:
                    // If it is a timeout error, warn the user and quit.
                    printf("Consumer %ld waited more than 3s for data.\n",
                            cons_num);
                    return NULL;
                    break;
                default:
                    // If another error happened, warn the user and quit.
                    printf("Consumer %ld received a bizarre error: %s.\n",
                            cons_num, strerror(errno));
                    return NULL;
                    break;
            }
        }

        // Sleep for 100us (simulating real work)...
        usleep(100);

        // Lock the job queue while this thread is accessing it.
        pthread_mutex_lock(&lock_job);
        // THIS_JOB is the job we are processing.
        this_job = job_queue;
        // We know JOB_QUEUE is not NULL because the semaphore was >0.
        job_queue = job_queue->next;
        // Release the lock so other threads can work on it.
        pthread_mutex_unlock(&lock_job);

        // Print the information stored in the job struct.
        printf("Consumer %ld got job %ld with payload %ld.\n",
                cons_num, this_job->index, this_job->payload);

        // Free the memory allocated for the current job.
        free(this_job);
    }

    // We never reach this point...
    return NULL;
}

/*
 * The producer creates a number of jobs that will be added to the queue
 * and then it announces that another job was added to the consumers
 * by posting (incrementing) to the semaphore.  The producer uses the
 * same mutex the consumers use to access the data in the job queue to
 * avoid race conditions.
 */
void *producer(void *arg)
{
    // The producer thread ID passed to the thread.
    long prod_num = (long) arg;
    // Job structure holding the new values.
    struct job *new_job;
    // How many jobs each producer thread produces.
    long jobs = 20;

    // Announces that the producer thread started.
    printf("Producer %ld started.\n", prod_num);

    // Loop and create each job.
    while (jobs-- > 0) {
        // Allocate memory for the new job.
        new_job = (struct job *)malloc(sizeof(struct job));

        // Lock the job queue for this thread.
        pthread_mutex_lock(&lock_job);

        // The next job is the last job.
        new_job->next = job_queue;
        // The job index is just the JOB_COUNTER, which increases for each job.
        new_job->index = job_counter++;
        // Uses a random number for the payload.
        new_job->payload = rand();
        // The head of the jobs list is the newly created job.
        job_queue = new_job;
        // Signals that a new job was added using the semaphore.
        sem_post(&job_q_count);

        // Release the lock.
        pthread_mutex_unlock(&lock_job);

        // Inform which data was added to the queue by this producer.
        printf("Producer %ld created job %ld with payload %ld.\n",
                prod_num, new_job->index, new_job->payload);
    }

    // Announces that the producer thread finished.
    printf("Producer %ld finished.\n", prod_num);

    // Exits.
    return NULL;
}

/*
 * Creates a number of consumers and producers threads.
 */
int main(int argc, char *argv[])
{
    // This is our producers and consumers threads IDs.
    pthread_t *prod_tid, *cons_tid;
    // How many consumers?
    long total_consumers = 4;
    // How many producers?
    long total_producers = 4;
    // Just a for counter...
    long i;

    // Alloc enough memory to store the producers and consumers thread IDs.
    prod_tid = (pthread_t *)malloc(total_producers * sizeof(pthread_t));
    cons_tid = (pthread_t *)malloc(total_consumers * sizeof(pthread_t));

    // Start the global semaphore with 0 and process-local (inter-threads
    // and not inter-processes).
    sem_init(&job_q_count, 0, 0);

    // Create the consumers which will wait on the producers data.
    for (i = 0; i < total_consumers; i++)
        pthread_create(&cons_tid[i], NULL, consumer, (void *) i + 1);

    // Wait before starting the producers.
    sleep(2);

    // Create the producers.
    for (i = 0; i < total_producers; i++)
        pthread_create(&prod_tid[i], NULL, producer, (void *) i + 1);

    // Join the producers.
    for (i = 0; i < total_producers; i++)
        pthread_join(prod_tid[i], NULL);

    // Join the consumers.
    for (i = 0; i < total_consumers; i++)
        pthread_join(cons_tid[i], NULL);

    // Free the memory used to store the threads IDs.
    free(prod_tid);
    free(cons_tid);

    // Destroy the mutex since we no longer use it.
    pthread_mutex_destroy(&lock_job);
    // ... and also the semaphore.
    sem_destroy(&job_q_count);

    // Everything went OK so, goodbye...
    return EXIT_SUCCESS;
}
