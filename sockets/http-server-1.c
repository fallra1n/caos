#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <signal.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/stat.h>

#define EXIT(code, msg) perror(msg); exit(code);
#define ERROR_HANDLE(code, error_code, msg) if(code == error_code) { EXIT(1, msg) }
#define ERROR_CODE (-1)
#define BUFFER_SIZE 4096

const char MESSAGE_200[] = "HTTP/1.1 200 OK\r\n";
const char MESSAGE_404[] = "HTTP/1.1 404 Not Found\r\n";
const char MESSAGE_403[] = "HTTP/1.1 403 Forbidden\r\n";
const char LAST_MESSAGE[] = "Content-Length: %ld\r\n\r\n";


sig_atomic_t is_broken = 0;

void HandleSignals(int signum __attribute__((unused))) {
  is_broken = 1;
}

void DefineHandlers() {
  struct sigaction action = {
      .sa_handler = HandleSignals
  };

  int ret_val = sigaction(SIGTERM, &action, NULL);
  ERROR_HANDLE(ret_val, ERROR_CODE, "sigaction")

  ret_val = sigaction(SIGINT, &action, NULL);
  ERROR_HANDLE(ret_val, ERROR_CODE, "sigaction")
}


void HandleClient(int fd, char* path_to_dir) {
  char buffer[BUFFER_SIZE];
  char filename[PATH_MAX];

  ssize_t count_of_bytes = read(fd, buffer, sizeof(buffer));
  ERROR_HANDLE(count_of_bytes, 0, "read")

  sscanf(buffer, "GET %s", filename);
  char abs_path[PATH_MAX];
  sprintf(abs_path, "%s/%s", path_to_dir, filename);

  if (access(abs_path, F_OK) == -1) {
    write(fd, MESSAGE_404, strlen(MESSAGE_404));
    dprintf(fd, LAST_MESSAGE, (long)0 );

  } else if (access(abs_path, R_OK) == -1) {
    write(fd, MESSAGE_403, strlen(MESSAGE_403));


    struct stat st;
    stat(abs_path, &st);
    dprintf(fd, LAST_MESSAGE, st.st_size);

  } else {
    write(fd, MESSAGE_200, strlen(MESSAGE_200));

    struct stat st;
    stat(abs_path, &st);
    dprintf(fd, LAST_MESSAGE, st.st_size);

    int fd_file = open(abs_path, O_RDONLY);
    char file_buf[BUFFER_SIZE];

    while ((count_of_bytes = read(fd_file, file_buf, BUFFER_SIZE)) > 0) {
      write(fd, file_buf, count_of_bytes);
    }

    close(fd_file);
  }

  shutdown(fd, SHUT_RDWR);
  close(fd);
}

int main(int argc, char **argv) {
  if (argc < 3) {
    return 1;
  }

  in_port_t port = htons(strtol(argv[1], NULL, 10));
  in_addr_t host_addr = inet_addr("127.0.0.1");
  char *path_to_dir = argv[2];

  struct sockaddr_in addr = {
      .sin_family = AF_INET,
      .sin_port = port,
      .sin_addr = host_addr
  };

  DefineHandlers();

  int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
  ERROR_HANDLE(sock_fd, ERROR_CODE, "socket")

  int ret_val = bind(sock_fd, (struct sockaddr*) &addr, sizeof(addr));
  ERROR_HANDLE(ret_val, ERROR_CODE, "bind")


  ret_val = listen(sock_fd, 50);
  ERROR_HANDLE(ret_val, ERROR_CODE, "listen")

  while (!is_broken) {
    int fd = accept(sock_fd, NULL, NULL);
    ERROR_HANDLE(fd, ERROR_CODE, "accept")

    HandleClient(fd, path_to_dir);

    shutdown(fd, SHUT_RDWR);
    close(fd);
  }

  close(sock_fd);

  return 0;
}