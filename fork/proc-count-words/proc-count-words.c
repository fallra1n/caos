#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdbool.h>

#define WORD_SIZE 4096

int main() {

    char cur_word[WORD_SIZE];
    int res = 0;
    int is_root = true;

    while (true) {
        pid_t pid = fork();

        if (pid == 0) {
            is_root = false;

            int count_of_symbols = scanf("%s", cur_word);

            if (count_of_symbols == EOF) {
                return res;
            } else {
                ++res;
            }
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