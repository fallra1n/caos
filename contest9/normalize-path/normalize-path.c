#include <stdio.h>
#include <stdlib.h>

void normalize_path(char *path) {
    char *write_ptr = path + 0;
    char *cur_ptr = path + 0;

    int count_of_slashs = 0;
    int count_of_dots = 0;

    while (*cur_ptr != '\0') {
        if (*cur_ptr == '/') {
            ++count_of_slashs;

            if (count_of_slashs <= 1 && count_of_dots != 1) {
                *write_ptr = '/';
                ++write_ptr;
            }

            if (count_of_dots == 1 && count_of_slashs == 1) {
                count_of_dots = 0;
                count_of_slashs = 0;
            }
        } else if (*cur_ptr == '.') {
            ++count_of_dots;

            if (count_of_dots == 2) {
                write_ptr -= 2;
                do {
                    --write_ptr;
                } while (*write_ptr != '/');

                count_of_dots = 0;
            } else {
                count_of_slashs = 0;
            }
        } else {
            *(write_ptr) = *(cur_ptr);
            ++write_ptr;
            count_of_dots = 0;
            count_of_slashs = 0;
        }

        ++cur_ptr;
    }

    *write_ptr = '\0';
}
