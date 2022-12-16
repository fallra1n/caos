#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdbool.h>

int main(int argc, char **argv) {
    if (argc < 3) {
        return 1;
    }

    int return_value = 0;

    char *file_name = argv[1];
    char *target = argv[2];

    int fd = open(file_name, O_RDONLY);

    struct stat st;
    
    if (fstat(fd, &st) == -1) {
        return_value = 1;
        goto Exit;
    }


    char *data = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

    if (data == MAP_FAILED) {
        return_value = 1;
        goto Exit;
    }

    char *cur_pos = data;
    int len = 0;

    while (len < st.st_size) {
        int target_size = strlen(target);
        bool flag = false;

        for (int i = 0; i < target_size; ++i) {
            if (target[i] != *(cur_pos + i)) {
                cur_pos += (i + 1);
                len += (i + 1);
                flag = true;
                break;
            }
        }

        if (flag) {
            continue;
        }

        size_t diff = cur_pos - data;
        printf("%lu ", diff);
        cur_pos += 1;
        len += 1;
    }

    printf("\n");

Exit:
    if (munmap(data, st.st_size) != 0) {
        return_value |= 1;
    }

    close(fd);
    return return_value;
}