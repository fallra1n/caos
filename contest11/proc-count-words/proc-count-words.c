#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdbool.h>

int main() {

    int ch;
    int res = 0;
    int is_root = true;

    while (true) {
        pid_t pid = fork();

        if (pid == 0) {
            is_root = false;

            ch = fgetc(stdin);

            while ((ch == ' ' || ch == '\n') && ch != EOF) {
                ch = fgetc(stdin);
            }

            if (ch == EOF) {
                return res;
            }

            while (!(ch == ' ' || ch == '\n') && ch != EOF) {
                ch = fgetc(stdin);
            }

            if (ch == EOF) {
                return res + 1;
            }

            ++res;
        }

        if (pid != 0) {
            int fork_ret;
            waitpid(pid, &fork_ret, 0);

            res = WEXITSTATUS(fork_ret);

            if (!is_root) {
                return res;
            }
            
            if (is_root) {
                break;
            }
        }
    }

    printf("\n%d\n", res);
    return 0;
}