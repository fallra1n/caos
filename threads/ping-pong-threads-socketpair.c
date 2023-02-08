#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

#define ERROR_HANDLE(code, expected) if(code != expected) { return_val = 1; goto Exit;}
#define SUCCESS_RET_CODE 0

typedef struct ThreadInfo {
  int thread_number;
  int fd;
  int another_fd;
} ThreadInfo;

void *PingPong(void *arg) {
  ThreadInfo *ti = arg;

  int cur_val;
  while (read(ti->fd, &cur_val, sizeof(int)) > 0) {
    if (ti->thread_number == 1) {
      cur_val -= 3;
    } else {
      cur_val += 5;
    }

    printf("%d\n", cur_val);

    if (cur_val <= 0 || cur_val > 100) {
      shutdown(ti->another_fd, SHUT_RDWR);
      shutdown(ti->fd, SHUT_RDWR);
      return NULL;
    } else {
      write(ti->fd, &cur_val, sizeof(int));
    }
  }

  return NULL;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    return 1;
  }

  int return_val = 0;
  int start_val = (int)strtol(argv[1], NULL, 10);

  pthread_t first_thread;
  pthread_t second_thread;

  int sockets_fd[2];
  // read write - SOCKSTREAM, threads from local machine - AF_UNIX
  socketpair(AF_UNIX, SOCK_STREAM, 0, sockets_fd);
  write(sockets_fd[1], &start_val,sizeof(int));

  int ret_code;

  ThreadInfo first_thread_info = {1, sockets_fd[0], sockets_fd[1]};
  ret_code = pthread_create(&first_thread, NULL, PingPong, &first_thread_info);
  ERROR_HANDLE(ret_code, SUCCESS_RET_CODE)

  ThreadInfo second_thread_info = {2, sockets_fd[1], sockets_fd[0]};
  ret_code = pthread_create(&second_thread, NULL, PingPong, &second_thread_info);
  ERROR_HANDLE(ret_code, SUCCESS_RET_CODE)

  ret_code = pthread_join(first_thread, NULL);
  ERROR_HANDLE(ret_code, SUCCESS_RET_CODE)

  ret_code = pthread_join(second_thread, NULL);
  ERROR_HANDLE(ret_code, SUCCESS_RET_CODE)

Exit:
  close(sockets_fd[0]);
  close(sockets_fd[1]);

  return return_val;
}