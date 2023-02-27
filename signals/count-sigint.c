#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define EXIT(code, msg) perror(msg); exit(code);
#define ERROR_HANDLE(code, error_code, msg) if(code == error_code) { EXIT(1, msg) }
#define ERROR_CODE (-1)

int sigint_count = 0;

void HandleSigint(int sig_number __attribute__((unused))) {
  sigint_count++;
}

void HandleTerm(int sig_number __attribute__((unused))) {
  printf("%d", sigint_count);
  fflush(stdout);
  exit(0);
}

int main() {
  struct sigaction action_int;
  memset(&action_int, 0, sizeof(action_int));
  action_int.sa_handler = HandleSigint;
  int ret_val = sigaction(SIGINT, &action_int, NULL);
  ERROR_HANDLE(ret_val, ERROR_CODE, "sigaction")

  struct sigaction action_term;
  memset(&action_term, 0, sizeof(action_term));
  action_term.sa_handler = HandleTerm;
  ret_val = sigaction(SIGTERM, &action_term, NULL);
  ERROR_HANDLE(ret_val, ERROR_CODE, "sigaction")

  printf("%d\n", getpid());
  fflush(stdout);

  for(;;) {
    pause();
  }
}