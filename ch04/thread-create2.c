#include <pthread.h>
#include <stdio.h>

struct char_print_params
{
    char character;
    int count;
};

void* char_print(void* params)
{
    struct char_print_params* p = (struct char_print_params*) params;
    int i;

    for (i = 0; i < p->count; i++)
        fputc(p->character, stderr);

    return NULL;
}

int main(void)
{
    pthread_t thread1_id;
    pthread_t thread2_id;
    struct char_print_params thread1_args;
    struct char_print_params thread2_args;

    thread1_args.character = 'o';
    thread1_args.count = 30000;
    pthread_create(&thread1_id, NULL, &char_print, &thread1_args);

    thread2_args.character = '-';
    thread2_args.count = 20000;
    pthread_create(&thread2_id, NULL, &char_print, &thread2_args);

    return 0;
}
