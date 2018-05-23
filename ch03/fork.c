#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(void)
{
    pid_t child_pid;

    printf ("the main program process ID is %d\n", (int) getpid());

    child_pid = fork();
    if (child_pid != 0) {
        printf("this is the parent process, with ID %d\n", (int) getpid());
        printf("the child's process ID is %d\n", (int) child_pid);
    } else {
        printf("this is the child process, with ID %d\n", (int) getpid());
        printf("the parent process ID is %d\n", (int) getppid());
    }

    return 0;
}
