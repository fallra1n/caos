#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        return 1;
    }

    int n = atoi(argv[1]);
    for (int i = 1; i <= n; ++i) {
        pid_t pid = fork();

        if (pid == 0) { // if child
            printf("%d", i);

            if (i == n) {
                printf("\n");
            } else {
                printf(" ");
            }
        } else { // if parent
            int fork_ret;
            waitpid(pid, &fork_ret, 0);

            if (fork_ret == -1) {
                return 1;
            }

            return 0;
        }
        fflush(stdout);
    }

    return 0;
}