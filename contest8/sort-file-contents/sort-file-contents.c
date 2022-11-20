#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

const int BUFFER_SIZE = 512 * 1024; // 1mb

int Compare(const void* lhs, const void* rhs) {
    int arg1 = *(const int*)lhs;
    int arg2 = *(const int*)rhs;
 
    if (arg1 < arg2) return -1;
    if (arg1 > arg2) return 1;
    return 0;
}

int Merge(int fd, int *fds, int len, int count_of_blocks, int len_of_last_block) {
    // back all pointers to start
    for (int i = 0; i < count_of_blocks; ++i) {
        int back_cur_ptr_to_start = lseek(fds[i], 0, SEEK_SET);
        if (back_cur_ptr_to_start == -1) {
            return 1;
        }
    }

    if (len_of_last_block != 0) {
        int back_ptr_to_start = lseek(fds[count_of_blocks], 0, SEEK_SET);
        if (back_ptr_to_start == -1) {
            return 1;
        }
    }


    int back_ptr_to_start = lseek(fd, 0 , SEEK_SET);
    if (back_ptr_to_start == -1) {
        return 1;
    }

    int buffer[BUFFER_SIZE / 4];
    int cur_buf_len = 0;

    for (int j = 0; j < len; ++j) {
        int min = INT32_MAX;
        int min_fd = 0;
        
        // search min among all blocks
        for (int i = 0; i < count_of_blocks; ++i) {
            int number;
            int count_read = read(fds[i], &number, sizeof(int));

            if (count_read == -1) {
                return 1;
            }

            if (count_read == 0) {
                continue;
            }

            if (number < min) {
                min = number;
                min_fd = fds[i];
            }

            int back_cur_ptr = lseek(fds[i], -4, SEEK_CUR);
            if (back_cur_ptr == -1) {
                return 1;
            }
        }

        // compare number of last block with cur min
        if (len_of_last_block != 0) {
            int last_number;
            int count_read = read(fds[count_of_blocks], &last_number, sizeof(int));

            if (count_read == -1) {
                return 1;
            }

            if (count_read != 0) {
                if (last_number < min) {
                    min = last_number;
                    min_fd = fds[count_of_blocks];
                }

                int back_cur_ptr = lseek(fds[count_of_blocks], -4, SEEK_CUR);
                if (back_cur_ptr == -1) {
                    return 1;
                }
            }
        }

        // add into cur buf min val
        buffer[cur_buf_len++] = min;
        int push_min_ptr = lseek(min_fd, 4, SEEK_CUR);
        if (push_min_ptr == -1) {
            return 1;
        }

        // write buffer to result file
        if (cur_buf_len == BUFFER_SIZE / 4) {

            cur_buf_len = 0;
            int write_count = write(fd, buffer, BUFFER_SIZE);
            if (write_count != BUFFER_SIZE) {
                return 1;
            }
        }
    }

    if (cur_buf_len != BUFFER_SIZE / 4) {
        int write_count = write(fd, buffer, cur_buf_len * 4);
        if (write_count != cur_buf_len * 4) {
            return 1;
        }
    }

    return 0;
}

int MergeSort(int fd, int len, int count_of_blocks, int len_of_last_block) {
    int fds[count_of_blocks + 1];
    int buffer[BUFFER_SIZE / 4];
    int last_buffer[len_of_last_block];

    int i = 0;
    for (i = 0; i < count_of_blocks; ++i) {
        int read_count = read(fd, buffer, BUFFER_SIZE);
        if (read_count != BUFFER_SIZE) {
            return 1;
        }
        qsort(buffer, BUFFER_SIZE / 4, sizeof(int), Compare);

        char file_name[10];
        sprintf(file_name, "%d", i);
        fds[i] = open(file_name, O_RDWR | O_CREAT | O_TRUNC, 0640);

        if(fds[i] == -1) {
            return 1;
        }

        int write_count = write(fds[i], buffer, BUFFER_SIZE);
        if (write_count != BUFFER_SIZE) {
            return 1;
        }
    }

    if (len_of_last_block != 0) {
        int read_count = read(fd, last_buffer, len_of_last_block);
        if (read_count != len_of_last_block) {
            return 1;
        }
        qsort(last_buffer, len_of_last_block / 4, sizeof(int), Compare);

        char file_name[10];
        sprintf(file_name, "%d", i);
        fds[i] = open(file_name, O_RDWR | O_CREAT | O_TRUNC, 0640);

        if(fds[i] == -1) {
            return 1;
        }

        read_count = write(fds[i], last_buffer, len_of_last_block);
        if (read_count != len_of_last_block) {
            return 1;
        }
    }

    int ret = Merge(fd, fds, len, count_of_blocks, len_of_last_block);
    if (ret != 0) {
        return 1;
    }

    // close all files
    for (int i = 0; i < count_of_blocks; ++i) {
        close(fds[i]);
    }

    if (len_of_last_block != 0) {
        close(fds[count_of_blocks]);
    }

    return 0;
}

int Testing(int fd, int count) {
    lseek(fd, 0, SEEK_SET);
    int buf[count / 4];
    read(fd, buf, count);

    int st[count / 4];
    int sfd = open("input_copy.bin", O_RDONLY, 0640);
    read(sfd, st, count);
    qsort(st, count / 4, sizeof(int), Compare);

    for(int i = 0; i < 100; ++i) {
        if (buf[i] != st[i]) {
            printf("%d:    %d ---------- %d\n",i, buf[i], st[i]);
        }
    }

    printf("YEEEEEEEEEEEEEEEEEES\n");
    return 0;
}

int main(int argc, char **argv) {
    if (argc < 1) {
        return 1;
    }

    char *first = argv[1];

    int return_value = 0;
    int fd = -1;

    fd = open(first, O_RDWR, 0640);
    if (fd == -1) {
        return_value = 1;
        goto Exit;
    }

    int count_bytes = lseek(fd, 0, SEEK_END);
    if (count_bytes == -1) {
        return_value = 1;
        goto Exit;
    }
    int len = count_bytes / sizeof(int);
    int count_of_blocks = count_bytes / BUFFER_SIZE;
    int len_of_last_block = count_bytes % BUFFER_SIZE;

    int back_ptr_to_start = lseek(fd, 0, SEEK_SET);
    if (back_ptr_to_start == -1) {
        return_value = 1;
        goto Exit;
    }

    int return_code = MergeSort(fd, len, count_of_blocks, len_of_last_block);
    return_value = return_code;
    // Testing(fd, count_bytes);

Exit:
    close(fd);

    return return_value;
}