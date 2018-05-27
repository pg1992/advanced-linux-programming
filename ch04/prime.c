#include <pthread.h>
#include <stdio.h>

/* Compute successive prime numbers (very inefficiently).  Return the
 * Nth prime number, where N is the value pointed to by *ARG. */

void* compute_prime(void* arg)
{
    long candidate = 2;
    long n = *((long*) arg);

    while (1) {
        int factor;
        int is_prime = 1;

        /* Test primality by successive division. */
        for (factor = 2; factor < candidate; factor++)
            if (candidate % factor == 0) {
                is_prime = 0;
                break;
            }

        /* Is this the prime number we're looking for? */
        if (is_prime)
            if (--n == 0)
                /* Return the desired prime number as the thread
                 * return value. */
                return (void*) candidate;

        ++candidate;
    }

    return NULL;
}

int main(void)
{
    pthread_t thread;
    long which_prime = 5000;
    long prime;

    /* Start the computing thread, up to the 5,000th prime number. */
    pthread_create(&thread, NULL, &compute_prime, (void*) &which_prime);
    /* Do some other work here... */
    /* Wait for the prime number thread to complete, and get the result. */
    if (!pthread_equal(pthread_self(), thread))
        pthread_join(thread, (void*) &prime);

    /* Print the largest prime number it computed. */
    printf("The %dth prime number is %d,\n", which_prime, prime);

    return 0;
}
