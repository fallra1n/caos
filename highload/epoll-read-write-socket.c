#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define EXIT(code, m_) perror(m_); exit(code);
#define ERROR_HANDLE(code, error_code, m_) if(code == error_code) { EXIT(1, m_) }
#define ERROR_CODE (-1)
#define MaxPendingEvents 10000

volatile sig_atomic_t interrupted = 0;

void HandleSigterm(int n __attribute__((unused))) {
  interrupted = 1;
}

void SetHandlers() {
  struct sigaction action = {
      .sa_handler = HandleSigterm
  };
  sigaction(SIGTERM, &action, NULL);
}

static void MakeNonBlocking(int fd) {
  fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

void CreateAcceptEvent(int epoll_fd, int socket_fd) {
  MakeNonBlocking(socket_fd);

  struct epoll_event event_ready_accept = {
      .events = EPOLLIN | EPOLLOUT | EPOLLET,
      .data.fd = socket_fd
  };

  epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket_fd, &event_ready_accept);
}

void CreateServeEvent(int epoll_fd, int conn_fd) {
  MakeNonBlocking(conn_fd);

  struct epoll_event event_ready_read = {
      .events = EPOLLIN,
      .data.fd = conn_fd
  };

  epoll_ctl(epoll_fd, EPOLL_CTL_ADD, conn_fd, &event_ready_read);
}

void ProcessRead(int conn_fd) {
  char buffer[BUFSIZ];
  ssize_t count = read(conn_fd, buffer, sizeof(buffer));
  if (count > 0) {
    for (size_t i = 0; i < count; i++) {
      if ('a' <= buffer[i] && buffer[i] <= 'z') {
        buffer[i] += 'A' - 'a';
      }
    }

    write(conn_fd, buffer, count);
  }
}

void ProcessEpollEvent(int epoll_fd, int socket_fd, struct epoll_event *event) {
  const uint32_t mask = event->events;

  if (mask & EPOLLERR) {
    EXIT(1, "epoll error")
  }

  if (event->data.fd == socket_fd) {
    int conn_fd = accept(socket_fd, NULL, NULL);
    if (conn_fd < 0) {
      return;
    }
    CreateServeEvent(epoll_fd, conn_fd);
  } else {
    int conn_fd = event->data.fd;
    if (mask & EPOLLIN) {
      ProcessRead(conn_fd);
    } else {
      EXIT(1, "masc cancel")
    }
  }

  if (mask & (EPOLLRDHUP | EPOLLHUP)) { // Close connection
    int conn_fd = event->data.fd;

    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, conn_fd, NULL);
    close(conn_fd);
  }
}

void StartServer(in_port_t port, in_addr_t ip_addr) {
  struct sockaddr_in addr = {
      .sin_family = AF_INET,
      .sin_port = port,
      .sin_addr = ip_addr
  };

  SetHandlers();

  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  ERROR_HANDLE(socket_fd, ERROR_CODE, "socket")

  int ret_val = bind(socket_fd, (struct sockaddr *) &addr, sizeof(addr));
  ERROR_HANDLE(ret_val, ERROR_CODE, "bind")

  ret_val = listen(socket_fd, SOMAXCONN);
  ERROR_HANDLE(ret_val, ERROR_CODE, "listen")

  int epoll_fd = epoll_create1(0);

  CreateAcceptEvent(epoll_fd, socket_fd);

  struct epoll_event pending[MaxPendingEvents];

  while (!interrupted) {
    int n = epoll_wait(epoll_fd, pending, MaxPendingEvents, -1);
    for (int i = 0; i < n; i++) {
      ProcessEpollEvent(epoll_fd, socket_fd, &pending[i]);
    }
  }

  close(socket_fd);
}

int main(int argc, char **argv) {
  if (argc < 2) {
    return 1;
  }

  in_port_t port = htons(strtol(argv[1], NULL, 10));
  in_addr_t ip_addr = inet_addr("127.0.0.1");

  StartServer(port, ip_addr);
  return 0;
}
