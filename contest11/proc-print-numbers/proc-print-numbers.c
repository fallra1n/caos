#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        return 1;
    }

    int n = strtol(argv[1], NULL, 10);

    for (int i = 1; i < n; ++i) {
        printf("%d ", i);
        fflush(stdout);

        uint32_t pid = fork();

        if (!pid) {
            break;
        }
    }

    return 0;
}