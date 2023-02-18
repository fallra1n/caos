#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#define EXIT(code, msg) perror(msg);  exit(code);
#define ERROR_HANDLE(code, error_code, msg) if(code == error_code) { EXIT(1, msg) }
#define ERROR_CODE (-1)
#define BUFFER_SIZE 65536 // 64kb

int main(int argc, char **argv) {
  if (argc < 3) {
    return 1;
  }

  char *cmd = argv[1];
  char *in = argv[2];

  int ret_code;
  int pipefd[2];
  ret_code = pipe(pipefd);
  ERROR_HANDLE(ret_code, ERROR_CODE, "failed to create a pipe")

  int in_fd = open(in, O_RDONLY);
  ERROR_HANDLE(in_fd, ERROR_CODE, "file opening error")

  ret_code = dup2(in_fd, 0);
  ERROR_HANDLE(ret_code, ERROR_CODE, "failed dup2")
  close(in_fd);

  int id = fork();
  if (id == 0) {
    ret_code = dup2(pipefd[1], 1);
    ERROR_HANDLE(ret_code, ERROR_CODE, "failed dup2")

    close(pipefd[1]);
    execlp(cmd, cmd, NULL);
    EXIT(1, "execlp failed")

  } else {
    close(pipefd[1]);

    ssize_t read_count = 0;
    ssize_t cur;
    char buf[BUFFER_SIZE];

    while ((cur = read(pipefd[0], buf, BUFFER_SIZE)) > 0) {
      read_count += cur;
    }

    int fork_ret;
    waitpid(id, &fork_ret, 0);
    ERROR_HANDLE(fork_ret, ERROR_CODE, "waiting error")

    printf("%zd\n", read_count);
  }

  return 0;
}