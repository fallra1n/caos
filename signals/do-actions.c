#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define EXIT(code, msg) perror(msg); exit(code);
#define ERROR_HANDLE(code, error_code, msg) if(code == error_code) { EXIT(1, msg) }
#define ERROR_CODE (-1)
#define NUMBER_OF_HANDLERS 4

volatile sig_atomic_t value = 0;

void HandleUsr1(int sig_number __attribute__((unused))) {
  value += 1;
  printf("%d\n", value);
  fflush(stdout);
}

void HandleUsr2(int sig_number __attribute__((unused))) {
  value *= -1;
  printf("%d\n", value);
  fflush(stdout);
}

void HandleTermAndSigint(int sig_number __attribute__((unused))) {
  fflush(stdout);
  exit(0);
}


int main() {
  struct sigaction actions[NUMBER_OF_HANDLERS];

  void(*handlers[])(int) = {HandleTermAndSigint, HandleTermAndSigint, HandleUsr1, HandleUsr2};
  int signals[] = {SIGINT, SIGTERM, SIGUSR1, SIGUSR2};
  struct sigaction signal_handler;

  for (size_t i = 0; i < NUMBER_OF_HANDLERS; i++) {
    signal_handler.sa_handler = handlers[i];
    int ret_val = sigaction(signals[i], &signal_handler, NULL);
    ERROR_HANDLE(ret_val, ERROR_CODE, "sigaction")
  }

  printf("%d\n", getpid());
  fflush(stdout);

  scanf("%d", &value);

  for(;;) {
    pause();
  }
}