#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char **argv)
{
    /* Get the UID of the current process.  Usually is the UID of the user
     * that executes the process. */
    uid_t proc_uid = getuid();
    int ret_val;
    /* The file we'll change the is specified in the first argument. */
    char *path = argv[1];

    /* Print the UID of the current process. */
    printf("process uid = %d\n", proc_uid);

    ret_val = chown(path, proc_uid, -1);
    if (ret_val != 0) {
        /* Save errno because it's clobbered by the next system call. */
        int error_code = errno;
        /* The operation didn't succeed; chown should return -1 on error. */
        assert(ret_val == -1);
        /* Check the value of errno, and take appropriate action. */
        switch (error_code) {
            case EPERM:           /* Permission denied. */
            case EROFS:           /* PATH is on a read-only file system. */
            case ENAMETOOLONG:    /* PATH is too long. */
            case ENOENT:          /* PATH does not exist. */
            case ENOTDIR:         /* A component of PATH is not a directory. */
            case EACCES:          /* A component of PATH is not accessible. */
                /* Something's is wrong with the file.
                 * Print an error message. */
                fprintf(stderr, "error changing ownership of %s: %s\n",
                        path, strerror(error_code));
                /* Don't end the program; perhaps give the user a chance to
                 * choose another file... */
                break;

            case EFAULT:
                /* PATH contains an invalid memory address.
                 * This is probably a bug. */
                abort();

            case ENOMEM:
                /* Ran out of kernel memory. */
                fprintf(stderr, "%s\n", strerror(error_code));
                exit(1);

            default:
                /* Some other, unexpected, error code.  We've tried to handle
                 * all possible error codes; if we've missed one,
                 * that's a bug! */
                abort();
        }
    }

    return EXIT_SUCCESS;
}
