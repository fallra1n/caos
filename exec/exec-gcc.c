#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

const char C_CODE[] = "#include <stdio.h>\n"
                      "int main(){\n"
                      "    printf(\"%s\", (%s));\n"
                      "    return 0;\n"
                      "}\n";

const int BUFFER_SIZE = 4096;

int main() {
    int return_value = 0;

    char buffer[BUFFER_SIZE];

    if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
        return 1;
    }

    char *feed_line_ptr = memchr(buffer, '\n', BUFFER_SIZE);
    if (feed_line_ptr) {
        *(feed_line_ptr + 1) = '\0';
    }

    int len = strlen(buffer);

    char c_code[sizeof(C_CODE) + strlen(buffer)];
    char program_name[] = "calculate_expression.c";
    char bin_name[] = "calculate_expression";

    int fd = open(program_name, O_WRONLY | O_CREAT, 0640);
    if (fd == -1) {
        return_value = 1;
        goto Exit;
    }

    snprintf(c_code, sizeof(C_CODE) + len - sizeof("%s"), C_CODE, "%d", buffer);
    if (write(fd, c_code, sizeof(C_CODE) + len - sizeof("%s")) == -1) {
        return_value = 1;
        goto Exit;
    }
    close(fd);

    // compile file
    int pid = fork();

    if (pid == 0) {
        execlp("gcc", "gcc", "-o", bin_name, program_name, NULL);
        perror("execlp compile file");
        return 1;
    } else {
        int fork_ret;
        waitpid(pid, &fork_ret, 0);

        if (fork_ret == -1) {
            return_value = 1;
            goto Exit;
        }
    }

    // run file
    pid = fork();
    if (pid == 0) {
        execl(bin_name, bin_name, NULL);
        perror("execlp run file");
        return 1;
    } else {
        int fork_ret;
        waitpid(pid, &fork_ret, 0);

        if (fork_ret == -1) {
            return_value = 1;
            goto Exit;
        }
    }

    remove(program_name);
    remove(bin_name);

Exit:
    return return_value;
}