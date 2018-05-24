#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

sig_atomic_t child_exit_status;
void
clean_up_child_process(int signal_number)
{
    /* Clean up the child process. */
    int status;
    wait(&status);
    /* Store its exit status in a global variable. */
    child_exit_status = status;
}

int
main(void)
{
    /* Handle SIGCHLD by calling clean_up_child_progress. */
    struct sigaction sigchld_action;
    memset(&sigchld_action, 0, sizeof(sigchld_action));
    sigchld_action.sa_handler = clean_up_child_process;
    sigaction(SIGCHLD, &sigchld_action, NULL);

    pid_t child_pid = fork();
    if (child_pid > 0) {
        printf("Parent is going to sleep for 10 secs.\n");
        sleep(10);
        printf("Parent woke up!\n");
    } else {
        printf("A child is born with no state of mind, "
               "blind to the ways of mankind.\n");
        sleep(2);
        printf("It was plain to see that your life was lost, "
               "you was cold and your body swung back and fort.\n");
    }

    return 0;
}
