#include <unistd.h>

int main(void) {
    char *message = "Hello World\n";
    char buffer[6];

    read(0, buffer, 6);     // 0 is the file descriptor for STDIN
    write(1, message, 12);  // 1 is the file descriptor for STDOUT
    write(2, message, 12);  // 2 is the file descriptor for STDERR
    write(1, buffer, 5);

    return 0;
}
