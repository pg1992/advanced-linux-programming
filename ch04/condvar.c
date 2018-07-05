#include <pthread.h>
#include <unistd.h>
#include <string.h>

#define MAX_LOOPS 10

static int thread_flag;
static int loop_count = 0;
static pthread_cond_t thread_flag_cv;
static pthread_mutex_t thread_flag_mutex;

void initialize_flag()
{
    /* Initialize the mutex and condition variable. */
    pthread_mutex_init(&thread_flag_mutex, NULL);
    pthread_cond_init(&thread_flag_cv, NULL);
    /* Initialize the flag value. */
    thread_flag = 0;
}

/* Print a message to STDOUT to simulate some real work. */
void do_work()
{
    const char *message = "I'm doing some work :)\n\0";
    write(STDOUT_FILENO, message, strlen(message));
}

/* Calls do_work repeatedly while the thread flag is set; blocks if
 * the flag is clear. */

void *working_thread(void* thread_arg)
{
    /* Loop infinitely. */
    while(loop_count < MAX_LOOPS) {
        /* Lock the mutex before accessing the flag value. */
        pthread_mutex_lock(&thread_flag_mutex);
        while(!thread_flag) {
            /* Announce that it will be blocked since the flag is not set. */
            const char *message = "Flag isn't set :(  [sleeping]\n\0";
            write(STDOUT_FILENO, message, strlen(message));
            /* The flag is clear.  Wait for a signal on the condition
             * variable, indicating that the flag value has changed.  When the
             * signal arrives and this thread unblocks, loop and check the
             * flag again. */
            pthread_cond_wait(&thread_flag_cv, &thread_flag_mutex);
        }
        /* When we've gotten here, we know the flag must be set.  Unlock
         * the mutex. */
        pthread_mutex_unlock(&thread_flag_mutex);
        do_work();
        /* Sleep for half a second. */
        usleep(500000);
    }
    return NULL;
}

/* Sets the value of the thread flag to FLAG_VALUE. */

void set_thread_flag(int flag_value)
{
    /* Lock the mutex before accessing the flag value. */
    pthread_mutex_lock(&thread_flag_mutex);
    /* Set the flag value, and then signal in case thread_function is
     * blocked, waiting for the flag to become set.  However,
     * thread_function can't actually check the flag until the mutex is
     * unlocked. */
    thread_flag = flag_value;
    pthread_cond_signal(&thread_flag_cv);
    /* Unlock the mutex. */
    pthread_mutex_unlock(&thread_flag_mutex);
}

/* Keep toggling the flag with a 1s period. */
void *toggling_thread(void *thread_arg)
{
    while (loop_count++ < MAX_LOOPS) {
        set_thread_flag(!thread_flag);
        sleep(1);
    }
}

int main(void)
{
    pthread_t toggle_tid, work_tid;

    /* Start the threads. */
    pthread_create(&work_tid, NULL, working_thread, NULL);
    pthread_create(&toggle_tid, NULL, toggling_thread, NULL);

    /* Join the threads (e.g., wait for them to finish). */
    pthread_join(toggle_tid, NULL);
    pthread_join(work_tid, NULL);

    /* Good bye ;) */
    return 0;
}
