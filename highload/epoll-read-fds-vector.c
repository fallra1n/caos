#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <sys/epoll.h>
#include <stdio.h>

#define MaxPendingEvents 10000

static void MakeNonBlocking(int fd) {
  fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

typedef struct {
  int     fd;
  size_t  count;
  bool    done;
} data_t;

static data_t* CreateEventData(int epoll_fd, int fd) {
  MakeNonBlocking(fd);
  data_t* data = calloc(1, sizeof(data_t));
  data->fd = fd;

  struct epoll_event event_ready_read;
  event_ready_read.events = EPOLLIN;
  event_ready_read.data.ptr = data;
  epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event_ready_read);

  return data;
}

static void ProcessPairReadyRead(data_t* data) {
  char buffer[BUFSIZ];
  ssize_t count = read(data->fd, buffer, sizeof(buffer));
  if (count > 0) {
    data->count += count;
  } else {
    data->done = true;
    close(data->fd); // reached EOF
  }
}

static void ProcessEpollEvent(struct epoll_event* event, size_t* files_left) {
  const uint32_t mask = event->events;
  data_t* data = event->data.ptr;

  if (mask & EPOLLIN) {
    ProcessPairReadyRead(data);
  }

  if (data->done) {
    (*files_left)--; // dec number of open fds
  }
}

size_t read_data_and_count(size_t N, int in[N]) {
  data_t** entries = calloc(N, sizeof(data_t*));

  int epoll_fd = epoll_create1(0);

  for (size_t i = 0; i < N; i++) {
    entries[i] = CreateEventData(epoll_fd, in[i]);
  }

  size_t files_left = N;
  struct epoll_event pending[MaxPendingEvents];
  while (files_left > 0) {
    int n = epoll_wait(epoll_fd, pending, MaxPendingEvents, -1);
    for (int i = 0; i < n; i++) {
      ProcessEpollEvent(&pending[i], &files_left);
    }
  }

  close(epoll_fd);

  size_t result = 0;
  for (size_t i = 0; i < N; i++) {
    result += entries[i]->count;
    free(entries[i]);
  }

  free(entries);

  return result;
}

