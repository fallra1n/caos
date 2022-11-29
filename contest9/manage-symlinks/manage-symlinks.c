#include <stdio.h>
#include <sys/stat.h>
#include <linux/limits.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <inttypes.h>
#include <stdlib.h>

int LinkHandle(char *file_name) {
    int res = 0;
        
    // get real path
    char real_path[PATH_MAX];
    char *res_path = realpath(file_name, real_path);

    if (res_path) {
        printf("%s\n", real_path);   
    } else {
        res = 1;
    }

    return res;
}

int RegularFileHandle(char *file_name) {
    char link_name[PATH_MAX];
    strcpy(link_name, "link_to_");
    strcat(link_name, file_name);

    char real_path[PATH_MAX];
    realpath(file_name, real_path);
    symlink(real_path, link_name);

    return 0;
}

int main() {
    int retunr_value = 0;

    char file_name[PATH_MAX]; // PATH_MAX - max size of file name 
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
            // if is reg
            if (S_ISREG(cur_stat.st_mode)) {
                if (RegularFileHandle(file_name) != 0) {
                    retunr_value = 1;
                    goto Exit;
                }
            } else if (S_ISLNK(cur_stat.st_mode)) {
                // if is link
                if (LinkHandle(file_name) != 0) {
                    retunr_value = 1;
                    goto Exit;
                }
            }
        }
    }

Exit:
    
    return retunr_value; 
}