#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

#define EXIT(code, msg) perror(msg); exit(code);
#define ERROR_HANDLE(code, error_code, msg) if(code == error_code) { EXIT(1, msg) }
#define ERROR_CODE (-1)

typedef struct Count {
  int errors;
  int warnings;
} count_t;

count_t GetAmountOfStrings() {
  count_t res = {0, 0};

  char *buf = NULL;
  size_t start_size = 0;

  int last_error_line = -1;
  int last_warning_line = -1;

  while ((int)getline(&buf, &start_size, stdin) != EOF) {
    int num_of_two_dots = 0;
    int temp = 0;

    for (int i = 0; i < start_size; i++) {
      if (buf[i] == ':') {
        num_of_two_dots++;

        if (num_of_two_dots == 1) {
          while (buf[++i] != ':') {
            temp *= 10;
            temp += (int)buf[i] - '0';
          }
          num_of_two_dots++;
        }


        if (num_of_two_dots == 3) {
          if (strncmp(buf + i + 2, "error", 5) == 0) {
            if (temp != last_error_line) {
              res.errors += 1;
              last_error_line = temp;
            }
          }

          if (strncmp(buf + i + 2, "warning", 7) == 0) {
            if (temp != last_warning_line) {
              res.warnings += 1;
              last_warning_line = temp;
            }
          }
        }
      }
    }

    free(buf);
    buf = NULL;
  }

  return res;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    return 1;
  }

  char *file_name = argv[1];
  int ret_code;

  int pipefd[2];
  ret_code = (pipe(pipefd));
  ERROR_HANDLE(ret_code, ERROR_CODE, "pipefd")

  int id = fork();
  if (id == 0) {
    ret_code = dup2(pipefd[1], 2);
    ERROR_HANDLE(ret_code, ERROR_CODE, "dup2")

    close(pipefd[0]);
    close(pipefd[1]);

    execlp("gcc", "gcc", file_name, NULL);
    EXIT(1, "execlp")
  }

  close(pipefd[1]);

  ret_code = dup2(pipefd[0], 0);
  ERROR_HANDLE(ret_code, ERROR_CODE, "dup2")
  close(pipefd[0]);

  count_t res = GetAmountOfStrings();
  printf("%d %d\n", res.errors, res.warnings);

  int fork_ret;
  waitpid(id, &fork_ret, 0);
  ERROR_HANDLE(fork_ret, ERROR_CODE, "waitpid")

  return 0;
}