#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

#define EXIT(code, msg) perror(msg); exit(code);
#define ERROR_HANDLE(code, error_code, msg) if(code == error_code) { EXIT(1, msg) }
#define ERROR_CODE (-1)

void HandleRtmin(int signum, siginfo_t* info, void* ctx __attribute__((unused))) {
  if (info->si_value.sival_int == 0) {
    exit(0);
  }

  union sigval sv;
  sv.sival_int = info->si_value.sival_int - 1;

  sigqueue(info->si_pid, signum, sv);
}


int main() {
  // create mask for blocking
  sigset_t mask;
  sigfillset(&mask);
  sigdelset(&mask, SIGRTMIN);
  sigprocmask(SIG_SETMASK, &mask, NULL);

  struct sigaction action_rtmin;
  memset(&action_rtmin, 0, sizeof(action_rtmin));
  action_rtmin.sa_flags = SA_RESTART | SA_SIGINFO;
  action_rtmin.sa_sigaction = HandleRtmin;
  int ret_val = sigaction(SIGRTMIN, &action_rtmin, NULL);
  ERROR_HANDLE(ret_val, ERROR_CODE, "sigaction")

  while (1) {
    sigsuspend(&mask);
  }
}