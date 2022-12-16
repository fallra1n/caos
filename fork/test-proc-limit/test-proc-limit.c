#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


int main() {
    int res = 0;

    pid_t pid = 0;

    while (pid != -1) {
        ++res;
        pid = fork();

        if (pid == -1) {
            printf("%d", res);

        } else if (pid > 0) { // if parent
            int fork_ret;
            waitpid(pid, &fork_ret, 0);

            if (fork_ret == -1) {
                return 1;
            }
            
            return 0;
        }    
    }

    return 0;
}