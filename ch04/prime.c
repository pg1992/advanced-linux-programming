#include <pthread.h>
#include <stdio.h>

void* compute_prime(void* arg)
{
    long candidate = 2;
    long n = *((long*) arg);

    while (1) {
        int factor;
        int is_prime = 1;

        for (factor = 2; factor < candidate; factor++)
            if (candidate % factor == 0) {
                is_prime = 0;
                break;
            }

        if (is_prime)
            if (--n == 0)
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

    pthread_create(&thread, NULL, &compute_prime, (void*) &which_prime);
    pthread_join(thread, (void*) &prime);

    printf("The %dth prime number is %d,\n", which_prime, prime);

    return 0;
}
