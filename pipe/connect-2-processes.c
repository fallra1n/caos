#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#define EXIT(code, msg) perror(msg); exit(code);
#define ERROR_HANDLE(code, error_code, msg) if(code == error_code) { EXIT(1, msg) }
#define ERROR_CODE (-1)

int main(int argc, char **argv) {
  if (argc < 3) {
    return 1;
  }

  char *cmd1 = argv[1];
  char *cmd2 = argv[2];

  int ret_code;
  int pipefd[2];
  ret_code = pipe(pipefd);
  ERROR_HANDLE(ret_code, ERROR_CODE, "failed to create a pipe")

  int id1 = fork();

  if (id1 == 0) {
    ret_code = dup2(pipefd[1], 1);
    ERROR_HANDLE(ret_code, ERROR_CODE, "failed dup2")

    close(pipefd[1]);
    close(pipefd[0]);
    execlp(cmd1, cmd1, NULL);
    EXIT(1, "execlp failed")
  }

  int id2 = fork();
  if (id2 == 0) {
    ret_code = dup2(pipefd[0], 0);
    ERROR_HANDLE(ret_code, ERROR_CODE, "failed dup2")

    close(pipefd[0]);
    close(pipefd[1]);
    execlp(cmd2, cmd2, NULL);
    EXIT(1, "execlp failed")
  }

  close(pipefd[0]);
  close(pipefd[1]);

  int fork_ret;
  waitpid(id1, &fork_ret, 0);
  ERROR_HANDLE(fork_ret, ERROR_CODE, "waiting error")

  waitpid(id2, &fork_ret, 0);
  ERROR_HANDLE(fork_ret, ERROR_CODE, "waiting error")

  return 0;
}
