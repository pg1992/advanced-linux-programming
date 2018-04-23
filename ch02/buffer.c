#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>

const char *program_name;

void print_usage(FILE *stream, int exit_code)
{
    fprintf(stream, "Usage: %s [ -f | --flush ]\n", program_name);
    fprintf(stream, "Options:\n");
    fprintf(stream, "    -f, --flush       Flush STDOUT every loop\n");
    fprintf(stream, "    -n, --number NUM  Loop NUM times\n");
    fprintf(stream, "    -t, --time USECS  Loop every USECS microseconds\n");
    fprintf(stream, "    -h, --help        Display this help and exit\n");

    exit(exit_code);
}

int main(int argc, char **argv)
{
    int i;
    int flush = 0;
    int next_option;
    unsigned int usecs = 1e6;
    unsigned int number = 1e6;
    const char *const short_options = "n:t:fh";
    const struct option long_options[] = {
        {    "time", 1, NULL, 't' },
        {  "number", 1, NULL, 'n' },
        {    "help", 0, NULL, 'h' },
        {   "flush", 0, NULL, 'f' },
        {      NULL, 0, NULL,  0  }
    };

    program_name = argv[0];

    while ((next_option = getopt_long(argc, argv,
                    short_options, long_options, NULL)) != -1) {
        switch (next_option) {
            case 'n':
                number = (unsigned int) atoi(optarg);
                break;
            case 't':
                usecs = (unsigned int) atoi(optarg);
                break;
            case 'f':
                flush = 1;
                break;
            case 'h':
                print_usage(stdout, 0);
            case '?':
                print_usage(stderr, 1);
            case -1:
                break;
            default:
                abort();
        }
    }

    for (i = 0; i < number; i++) {
        fprintf(stdout, "*");  // STDOUT is buffered
        fprintf(stderr, ".");
        if (flush)
            fflush(stdout);
        usleep(usecs);
    }
    printf("\n");

    return EXIT_SUCCESS;
}
