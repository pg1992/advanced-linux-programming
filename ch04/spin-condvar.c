#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_LOOP 10

static int thread_flag = 1;
static pthread_mutex_t thread_flag_mutex;
static int loop_count = 0;

void initialize_flag()
{
    pthread_mutex_init(&thread_flag_mutex, NULL);
    thread_flag = 0;
}

void do_work(void) {
    printf("Flag is set... doing some work...\n");
}

/* Call do_work repeatedly while the thread flag is set; otherswise
 * spins. */

void *thread_function (void *thread_arg)
{
    while (loop_count < MAX_LOOP) {
        int flag_is_set;

        /* Protect the flag with a mutex lock. */
        pthread_mutex_lock(&thread_flag_mutex);
        flag_is_set = thread_flag;
        pthread_mutex_unlock(&thread_flag_mutex);

        if (flag_is_set)
            do_work();
        else
            printf("Flag is not set... looping again (wasting CPU)...\n");
        /* Else don't do anything.  Just loop again. */
    }
    return NULL;
}

/* Sets the value of the thread flag to FLAG_VALUE. */

void set_thread_flag (int flag_value)
{
    /* Protect the flag with a mutex lock. */
    pthread_mutex_lock(&thread_flag_mutex);
    thread_flag = flag_value;
    pthread_mutex_unlock(&thread_flag_mutex);
}

/* Periodically toggle the flag making the other thread alterate between
 * doing work and wating CPU looping while the flag is not set. */
void *thread_periodic_set (void *thread_arg)
{
    while (loop_count++ < MAX_LOOP) {
        printf("Toggling flag...\n");
        set_thread_flag(!thread_flag);
        sleep(1);
    }
    return NULL;
}

int main(void)
{
    pthread_t looping_thread, setting_thread;

    /* Create a thread that will loop and only do useful work when a flag
     * is set, while another thread keeps toggling the flag. */
    pthread_create(&looping_thread, NULL, thread_function, NULL);
    pthread_create(&setting_thread, NULL, thread_periodic_set, NULL);

    pthread_join(looping_thread, NULL);
    pthread_join(setting_thread, NULL);

    return 0;
}
