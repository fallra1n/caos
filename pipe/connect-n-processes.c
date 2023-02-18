#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#define EXIT(code, msg) perror(msg); exit(code);
#define ERROR_HANDLE(code, error_code, msg) if(code == error_code) { EXIT(1, msg) }
#define ERROR_CODE (-1)

typedef struct PipeFd {
  int in;
  int out;
} pipefd_t;

void CloseAllDiscriptors(pipefd_t fds[], int size) {
  for (int i = 0; i < size; i++) {
    close(fds[i].in);
    close(fds[i].out);
  }
}

void WaitAllChildren(int ids[], int size) {
  for (int i = 0; i < size; i++) {
    int fork_ret;
    waitpid(ids[i], &fork_ret, 0);
    ERROR_HANDLE(fork_ret, ERROR_CODE, "waitpid")
  }
}

int main(int argc, char **argv) {
  char **cmds = argv;
  argc -= 1;

  pipefd_t *pipes = malloc((argc - 1) * sizeof(pipefd_t));
  int *forks = malloc(argc * sizeof(int));

  for (int i = 0; i < argc - 1; i++) {
    int pipefd[2];
    int ret_code;
    ret_code = pipe(pipefd);
    ERROR_HANDLE(ret_code, ERROR_CODE, "pipe")

    pipes[i].in = pipefd[0];
    pipes[i].out = pipefd[1];
  }

  for (int i = 0; i < argc; i++) {
    int id = fork();

    if (id == 0) {
      if (i > 0) {
        int ret_code = dup2(pipes[i - 1].in, 0);
        ERROR_HANDLE(ret_code, ERROR_CODE, "dup2")
      }

      if (i < argc - 1) {
        int ret_code = dup2(pipes[i].out, 1);
        ERROR_HANDLE(ret_code, ERROR_CODE, "dup2")
      }

      CloseAllDiscriptors(pipes, argc - 1);

      execlp(cmds[i + 1], cmds[i + 1], NULL);
      EXIT(1, "execlp")
    }

    forks[i] = id;
  }

  CloseAllDiscriptors(pipes, argc - 1);
  WaitAllChildren(forks, argc);

  free(pipes);
  free(forks);
  return 0;
}