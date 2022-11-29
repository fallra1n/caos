#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

struct Item {
    int value;
    uint32_t next_pointer;
};

int main(int argc, char **argv) {
    if (argc < 2) {
        return 1;
    }

    char *first = argv[1];

    int return_value = 0;
    int read_fd = -1;

    read_fd = open(first, O_RDONLY);
    if (read_fd == -1) {
        return_value = 1;
        goto Exit;
    }

    struct Item item;

    do {
        int read_count = read(read_fd, &item, sizeof(item));
        if (read_count == 0) {
            goto Exit;
        }

        if (read_count == -1) {
            return_value = 1;
            goto Exit;
        }

        printf("%d\n", item.value);
        int offset = lseek(read_fd, item.next_pointer, SEEK_SET);
        if (offset == -1) {
            return_value = -1;
            goto Exit;
        }

    } while (item.next_pointer != 0);

Exit:
    close(read_fd);

    return return_value;
}