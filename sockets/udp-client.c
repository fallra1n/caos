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

  in_port_t port = htons(strtol(argv[1], NULL, 10));

  struct sockaddr_in full_addr = {
      .sin_family = AF_INET,
      .sin_addr.s_addr = inet_addr("127.0.0.1"),
      .sin_port = port
  };

  int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
  ERROR_HANDLE(sock_fd, ERROR_CODE, "socket")

  int ret_val = bind(sock_fd, (struct sockaddr*) &full_addr, sizeof(struct sockaddr_in));
  //ERROR_HANDLE(ret_val, ERROR_CODE, "bind")

  int send_num, reqv_num;
  while (scanf("%d", &send_num) > 0) {
    ret_val = sendto(sock_fd, &send_num, sizeof(int), 0, (struct sockaddr*) &full_addr, sizeof(full_addr));
    ERROR_HANDLE(ret_val, ERROR_CODE, "sendto")

    ret_val = recvfrom(sock_fd, &reqv_num, sizeof(int), 0, NULL, NULL);
    ERROR_HANDLE(ret_val, ERROR_CODE, "recvfro")

    printf("%d\n", reqv_num);
    fflush(stdout);
  }

  close(sock_fd);

  return 0;
}
