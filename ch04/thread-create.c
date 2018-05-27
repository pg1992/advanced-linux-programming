#include <pthread.h>
#include <stdio.h>

/* Print o's to stderr.  The parameter is unused.  Does not return. */

void *print_os(void *unused)
{
    while (1)
        fputc('o', stderr);
    return NULL;
}

/* The main program. */

int main()
{
    pthread_t thread_id;
    /* Create a new thread.  The new thread will run the print_os
     * function. */
    pthread_create(&thread_id, NULL, &print_os, NULL);
    /* Print -'s continuously to stderr. */
    while (1)
        fputc('-', stderr);
    return 0;
}
