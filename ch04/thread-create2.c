#include <pthread.h>
#include <stdio.h>

/* Parameters the CHAR_PRINT. */

struct char_print_params
{
    char character;
    int count;
};

/* Prints a number of characters to stderr, as given by PARAMETERS,
 * which is a pointer to a struct CHAR_PRINT_PARAMS. */

void* char_print(void* params)
{
    /* Cast the cookie pointer to the right type. */
    struct char_print_params* p = (struct char_print_params*) params;
    int i;

    for (i = 0; i < p->count; i++)
        fputc(p->character, stderr);

    return NULL;
}

/* The main program. */

int main(void)
{
    pthread_t thread1_id;
    pthread_t thread2_id;
    struct char_print_params thread1_args;
    struct char_print_params thread2_args;

    /* Create a new thread to print 30,000 'o's. */
    thread1_args.character = 'o';
    thread1_args.count = 30000;
    pthread_create(&thread1_id, NULL, &char_print, &thread1_args);

    /* Create a new thread to print 20,000 '-'s. */
    thread2_args.character = '-';
    thread2_args.count = 20000;
    pthread_create(&thread2_id, NULL, &char_print, &thread2_args);

    /* Make sure the first thread has finished. */
    pthread_join(thread1_id, NULL);
    /* Make sure the second thread has finished. */
    pthread_join(thread2_id, NULL);

    /* Now we can safely return. */
    return 0;
}
