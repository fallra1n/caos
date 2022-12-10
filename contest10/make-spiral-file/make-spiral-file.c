#include <stdlib.h>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>


enum direction {left, right, up, down};

void MakeCell(char *cell, int64_t num, int64_t width, bool is_last) {
    int64_t i = width - 1;
    do {
        cell[i--] = (char)(num % 10) + '0';
        num /= 10;
    } while (num > 0);

    while(i >= 0) {
        cell[i--] = ' ';
    }

    if (is_last) {
        cell[width] = '\n';
    }
}

void FillFileAsSpiral(char *data, int64_t size, int64_t width) {
    int cur_num = 1;

    int64_t left_size = size;
    int64_t right_size = size;
    int64_t down_size = size;
    int64_t up_size = size;

    enum direction write_direction = right;
    int i = 0, j = 0;
    int old_i, old_j;

    while (left_size > 0 || right_size > 0 || down_size > 0 || up_size > 0) {
        switch (write_direction) {

            case right:
                old_j = j;

                for (; j < right_size + old_j; ++j) {

                    if (j == size - 1) {
                        char cell[width + 1];
                        char last = '\n';

                        MakeCell(cell, cur_num, width, true);
                        memcpy(data + i * (width * size + 1) + j * width, cell, width + 1);
                    } else {
                        char cell[width];
                        MakeCell(cell, cur_num, width, false);
                        memcpy(data + i * (width * size + 1) + j * width, cell, width);
                    }

                    ++cur_num;
                }

                --j;
                ++i;
                --up_size;
                --down_size;
                write_direction = down;
                break;

            case down:
                old_i = i;

                for (; i < down_size + old_i; ++i) {

                    if (j == size - 1) {
                        char cell[width + 1];

                        MakeCell(cell, cur_num, width, true);
                        memcpy(data + i * (width * size + 1) + j * width, cell, width + 1);
                    } else {
                        char cell[width];
                        MakeCell(cell, cur_num, width, false);
                        memcpy(data + i * (width * size + 1) + j * width, cell, width);
                    }

                    ++cur_num;
                }

                --i;
                --j;
                --right_size;
                --left_size;
                write_direction = left;
                break;

            case left:
                old_j = j;
                for (; j > old_j - left_size; --j) {
                    char cell[width];
                    MakeCell(cell, cur_num, width, false);
                    memcpy(data + i * (width * size + 1) + j * width, cell, width);

                    ++cur_num;
                }

                --down_size;
                --up_size;
                ++j;
                --i;
                write_direction = up;
                break;

            case up:
                old_i = i;
                for (; i > old_i - up_size; --i) {
                    char cell[width];
                    MakeCell(cell, cur_num, width, false);
                    memcpy(data + i * (width * size + 1) + j * width, cell, width);

                    ++cur_num;
                }

                --right_size;
                --left_size;
                ++i;
                ++j;
                write_direction = right;
                break;
        }
    }
}

int main(int argc, char **argv) {
    if (argc < 4) {
        return 1;
    }

    int return_value = 0;

    char *file_name = argv[1];
    int64_t size = strtol(argv[2], NULL, 10);
    int64_t width = strtol(argv[3], NULL, 10);

    if (size == LONG_MIN || size == LONG_MAX || width == LONG_MIN || width == LONG_MAX) {
        return_value = 1;
        goto Exit;
    }

    int64_t file_size = size * size * width + size; // + size, count of '\n' + '\0';

    int fd = open(file_name,  O_RDWR | O_CREAT, 0640);
    if (fd == -1) {
        return_value = 1;
        goto Exit;
    }

    if (ftruncate(fd, file_size) == -1) {
        return_value = 1;
        goto Exit;
    }

    struct stat st;
    if (fstat(fd, &st) == -1) {
        return_value = 1;
        goto Exit;
    }

    char *data = mmap(NULL, file_size, PROT_WRITE, MAP_SHARED, fd, 0);
    if (data == MAP_FAILED) {
        return_value = 1;
        goto Exit;
    }

    FillFileAsSpiral(data, size, width);

Exit:
    close(fd);
    if (munmap(data, st.st_size) != 0) {
        return_value = 1;
    }
    return return_value;
}