#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

bool IsNumber(char *str, int len) {
    bool flag = true;

    for (int i = 0; i < len; ++i) {
        if (str[i] < '0' || str[i] > '9') {
            flag = false;
            break;
        }
    }

    if (len == 0) {
        flag = false;
    }

    return flag;
}

int main() {
    char *lines = Lines;
    int len = strlen(lines);

    int sum = 0;

    char *cur = (char *) malloc(len * sizeof(char));
    int cur_len = 0;
    bool isFirst = true;

    for (int i = 0; i < len; ++i) {
        if ((i < len - 1 && lines[i] == ' ' && lines[i + 1] == ':') || lines[i] == ':') {
            if (IsNumber(cur, cur_len)) {

                cur[cur_len] = '\0';
                sum += atoi(cur);
                isFirst = false;

            } else {
                if (!isFirst) {
                    printf("%s\n", cur);
                }
                isFirst = false;
            }

            cur = "";
            cur_len = 0;
            cur = (char *) malloc(len * sizeof(char));

            if (lines[i] != ':') {
                ++i;
            }
            continue;
        }
        cur[cur_len++] = lines[i];
    }

    if (IsNumber(cur, cur_len)) {
        cur[cur_len] = '\0';
        sum += atoi(cur);
    } else {
        if (!isFirst) {
            printf("%s\n", cur);
        }
    }

    printf("%d", sum);
    return 0;
}
