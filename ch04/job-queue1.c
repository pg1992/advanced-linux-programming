#include <malloc.h>

struct job {
    struct job* next;
};

struct job* job_queue;

void* thread_function(void* arg)
{
    while (job_queue != NULL) {
        struct job* next_job = job_queue;
        job_queue = job_queue->next;
        process_job(next_job);
        free(next_job);
    }
    return NULL;
}
