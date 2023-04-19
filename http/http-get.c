#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#define EXIT(code, msg) perror(msg); exit(code);
#define ERROR_HANDLE(code, error_code, msg) if(code == error_code) { EXIT(1, msg) }
#define ERROR_CODE (-1)

#define REQUEST_SIZE 4096
#define BUFFER_SIZE 65536

int main(int argc, char* argv[]) {
  if (argc < 3) {
    EXIT(ERROR_CODE, "argc")
  }

  signal(SIGPIPE, SIG_IGN);

  char* host_name = argv[1];
  char* path = argv[2];

  struct addrinfo addr_hints = {
      .ai_family = AF_INET,
      .ai_socktype = SOCK_STREAM};
  struct addrinfo* addr_result = NULL;
  getaddrinfo(host_name, "http", &addr_hints, &addr_result);

  int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
  ERROR_HANDLE(sock_fd, ERROR_CODE, "socket")

  int ret_val = connect(sock_fd, addr_result->ai_addr, addr_result->ai_addrlen);
  ERROR_HANDLE(ret_val, ERROR_CODE, "connect")

  char request[REQUEST_SIZE];
  snprintf(request, sizeof(request),
           "GET %s HTTP/1.1\r\n"
           "Host: %s\r\n"
           "Connection: close\r\n\r\n",
           path, host_name);

  ret_val = write(sock_fd, request, strnlen(request, sizeof(request)));
  ERROR_HANDLE(ret_val, ERROR_CODE, "write")

  FILE* in = fdopen(sock_fd, "r");
  char buf[BUFFER_SIZE];
  bool headers_completed = false;
  while (fgets(buf, sizeof(buf), in)) {
    if (strcmp(buf, "\n") == 0 || strcmp(buf, "\r\n") == 0) {
      headers_completed = true;
      continue;
    }
    if (headers_completed) {
      printf("%s", buf);
    }
  }

  close(sock_fd);
  fclose(in);

  return 0;
}