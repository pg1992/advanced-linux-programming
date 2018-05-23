#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

sig_atomic_t sigint_count = 0;

void sigint_handler(int signum)
{
    ++sigint_count;
}

int main(int argc, char **argv)
{
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = sigint_handler;
    sigaction(SIGINT, &sa, NULL);
    int num_times = argc > 1 ? atoi(argv[1]) : 10;
    int main_count = -1;

    while (sigint_count < num_times) {
        if (main_count != (int) sigint_count) {
            main_count = (int) sigint_count;
            printf("%d ctrl-c's to go\n", num_times - main_count);
        }
    }

    return 0;
}
