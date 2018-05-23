#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

sig_atomic_t sigusr1_count = 0;

void handler(int signal_number)
{
    ++sigusr1_count;
}

int main(void) {
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &handler;
    sigaction(SIGUSR1, &sa, NULL);

    /* Wait while 10 SIGUSR1 are raised.
     * This would do the trick:
     * for i in `seq 10` ; do kill -10 <pid> ; done */
    while (sigusr1_count < 10) ;

    printf("SIGUSR1 was raised %d times\n", sigusr1_count);
    return 0;
}
