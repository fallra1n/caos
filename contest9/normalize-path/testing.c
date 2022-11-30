#include <stdio.h>

extern void normalize_path(char* path);

int main() {
    char *path;
    scanf("%s", path);
    normalize_path(path);
    printf("%s", path);
    return 0;
}