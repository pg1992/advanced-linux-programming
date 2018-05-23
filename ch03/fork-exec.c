#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int spawn(char *command, char **arg_list)
{
    pid_t child_pid;

    child_pid = fork();
    if (child_pid != 0)
        return child_pid;
    else {
        execvp(command, arg_list);
        fprintf(stderr, "an error occurred in execvp\n");
        abort();
    }
}

int main(void)
{
    char *arg_list[] = {
        "ls",
        "-l",
        "/",
        NULL
    };

    spawn("ls", arg_list);

    printf("done with main program\n");

    return 0;
}
