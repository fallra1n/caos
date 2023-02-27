#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

#define EXIT(code, msg) perror(msg); exit(code);
#define ERROR_HANDLE(code, error_code, msg) if(code == error_code) { EXIT(1, msg) }
#define ERROR_CODE (-1)
#define MAX_NUMBER_OF_FILES 128

volatile sig_atomic_t cur_file = 0;
FILE* files[MAX_NUMBER_OF_FILES];


void HandlerFunc(int signum) {
  cur_file = signum - SIGRTMIN;
}

int main(int argc, char **argv) {
  int n = argc;

  // files open
  for (int i = 1; i < n; i++) {
    files[i] = fopen(argv[i], "r");
  }

  // create mask for blocking
  sigset_t mask;
  sigfillset(&mask);

  for (int i = SIGRTMIN; i <= SIGRTMAX; ++i) {
    sigdelset(&mask, i);
  }

  sigprocmask(SIG_SETMASK, &mask, NULL);

  // sig action
  struct sigaction signal_action;
  memset(&signal_action, 0, sizeof(signal_action));
  signal_action.sa_flags = SA_RESTART;
  signal_action.sa_handler = HandlerFunc;

  for (int i = SIGRTMIN; i <= SIGRTMAX; ++i) {
    int ret_val = sigaction(i, &signal_action, NULL);
    ERROR_HANDLE(ret_val, ERROR_CODE, "sigaction")
  }

  // handle real time signals
  while (1) {
    sigsuspend(&mask);
    if (cur_file == 0) {
      break;
    } else {
      char *buf = NULL;
      size_t start_size = 0;
      getline(&buf, &start_size, files[cur_file]);
      printf("%s", buf);
      fflush(stdout);
    }
  }

  for (int i = 1; i < n; ++i) {
    fclose(files[i]);
  }

  return 0;
}