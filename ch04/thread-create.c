#include <pthread.h>
#include <stdio.h>

void *print_os(void *unused)
{
    while (1)
        fputc('o', stderr);
    return NULL;
}

int main()
{
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, &print_os, NULL);
    while (1)
        fputc('-', stderr);
    return 0;
}
