#include <stdio.h>
#include <sys/stat.h>
#include <linux/limits.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <inttypes.h>


int main() {
    char file_name[PATH_MAX]; // PATH_MAX - max size of file name 
    uint64_t res = 0;
    struct stat cur_stat;

    // fgets - ret file_name or NULL
    while (fgets(file_name, sizeof(file_name), stdin)) {
        char *feed_line_ptr = memchr(file_name, '\n', sizeof(file_name));

        // change \n -> \0
        if (feed_line_ptr) {
            *feed_line_ptr = '\0';
        }

        // lstat - ret 0 or -1
        if (lstat(file_name, &cur_stat) != -1) {
            // S_ISREG = ret 0 - false or !0 - true
            if (S_ISREG(cur_stat.st_mode)) {
                res += cur_stat.st_size;
            }
        };
    }
    
    printf("%"PRIu64"\n", res);
    
    return 0; 
}