#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>

#define EXIT(code, msg) perror(msg); exit(code);
#define ERROR_HANDLE(code, error_code, msg) if(code == error_code) { EXIT(1, msg) }
#define ERROR_CODE (-1)

int main(int argc, char **argv) {
  if (argc < 2) {
    return 1;
  }

  in_addr_t addr = inet_addr(argv[1]);
  in_port_t port = htons(strtol(argv[2], NULL, 10));

  struct sockaddr_in full_addr = {
      .sin_family = AF_INET,
      .sin_port = port,
      .sin_addr = addr
  };

  int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
  ERROR_HANDLE(sock_fd, ERROR_CODE, "socket")

  int ret_val = connect(sock_fd, (const struct sockaddr *) &full_addr, sizeof(full_addr));
  ERROR_HANDLE(ret_val, ERROR_CODE, "connect")

  int to_send;
  int to_get;


  while (scanf("%d", &to_send) != EOF) {
    if (write(sock_fd, &to_send, sizeof(to_send)) <= 0) {
      break;
    }
    if (read(sock_fd, &to_get, sizeof(to_get)) <= 0) {
      break;
    }
    printf("%d ", to_get);
    fflush(stdout);
  }

  close(sock_fd);
  return 0;
}
