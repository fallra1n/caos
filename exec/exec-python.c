#include <stdio.h>
#include <string.h>
#include <unistd.h>

const int BUFFER_SIZE = 4096;

int main() {
    char buffer[BUFFER_SIZE];
    if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
        return 1;
    }

    char *feed_line_ptr = memchr(buffer, '\n', BUFFER_SIZE);
    if (feed_line_ptr) {
        *feed_line_ptr = '\0';
    }

    char python_expression[BUFFER_SIZE + 7];
    snprintf(python_expression, BUFFER_SIZE + 7, "print(%s)", buffer);

    execlp("python", "python", "-c", python_expression, NULL);
    perror("execlp");
    return 1;
}