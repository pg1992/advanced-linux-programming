#include <stdio.h>

/* The ENVIRON variable contains the environment. */
extern char **environ;

int main()
{
    char **var;
    for (var = environ; *var; var++)
        printf("%s\n", *var);
    return 0;
}
