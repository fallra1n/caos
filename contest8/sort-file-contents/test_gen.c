#include <fcntl.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv) {
    srand(time(NULL)); // Initialization, should only be called once.
    char *first = argv[1];

    int write_fd = -1;

    write_fd = open(first, O_WRONLY | O_CREAT, 0640);
    if (write_fd == -1) {
        return 1;
    }

    for (int i = 0; i < 1024 + 512; ++i) {
        int value = 512 * 1024 + 512 - i;
        int count = write(write_fd, &value, 4);

        if (count <= 0) {
            return 1;
        }
    }

    close(write_fd);
    return 0;
}