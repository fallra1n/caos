#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <signal.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>

#define EXIT(code, m_) perror(m_); exit(code);
#define ERROR_HANDLE(code, error_code, m_) if(code == error_code) { EXIT(1, m_) }
#define ERROR_CODE (-1)

#define PING_PKT_S 64

volatile sig_atomic_t interrupted = 0;

struct ping_pkt_t {
  struct icmphdr hdr;
  char message[PING_PKT_S - sizeof(struct icmphdr)];
};

void AlarmHandler(int sig_num __attribute__((unused))) {
  interrupted = 1;
}

void SetHandlers() {
  struct sigaction action = {
      .sa_handler = AlarmHandler
  };
  sigaction(SIGALRM, &action, NULL);
}

uint16_t ICMPChecksum( void *buf, int len) {
  uint16_t *icmph = buf;
  uint16_t ret = 0;
  uint32_t sum = 0;
  uint16_t odd_byte;

  while (len > 1) {
    sum += *icmph++;
    len -= 2;
  }

  if (len == 1) {
    *(uint8_t*)(&odd_byte) = * (uint8_t*)icmph;
    sum += odd_byte;
  }

  sum =  (sum >> 16) + (sum & 0xffff);
  sum += (sum >> 16);
  ret =  ~sum;

  return ret;
}

void SendPing(int sock_fd, struct sockaddr* conn_addr, size_t addr_conn_len, size_t timeout, size_t interval) {
  alarm(timeout); // alarm after timeout

  int ret_val = bind(sock_fd, conn_addr, addr_conn_len);
  //ERROR_HANDLE(ret_val, ERROR_CODE, "bind")

  size_t responses = 0;
  int msg_count = 0;

  struct ping_pkt_t pckt;

  while (!interrupted) {
    bzero(&pckt, sizeof(pckt));
    pckt.hdr.type = ICMP_ECHO;
    pckt.hdr.un.echo.id = getpid();

    int i;
    for (i = 0; i < sizeof(pckt.message) - 1; i++) {
      pckt.message[i] = (i) + '0';
    }

    pckt.message[i] = 0;
    pckt.hdr.un.echo.sequence = msg_count++;
    pckt.hdr.checksum = ICMPChecksum(&pckt, sizeof(struct ping_pkt_t));

    ret_val = sendto(sock_fd, &pckt, sizeof(pckt), 0, (struct sockaddr*) conn_addr, addr_conn_len);
    //ERROR_HANDLE(ret_val, ERROR_CODE, "sendto")

    ret_val = recvfrom(sock_fd, &pckt, sizeof(pckt), 0, NULL, NULL);
    //ERROR_HANDLE(ret_val, ERROR_CODE, "recvfrom")

    ++responses;
    usleep(interval); // wait
  }

  printf("%ld\n", responses);
}

int main(int argc, char **argv) {
  if (argc < 4) {
    return 1;
  }

  in_addr_t addr = inet_addr(argv[1]);
  in_port_t port = htons(0);

  size_t timeout = strtol(argv[2], NULL, 10);
  size_t interval = strtol(argv[3], NULL, 10);

  if (timeout == 0) {
    printf("0\n");
    return 0;
  }

  SetHandlers();

  struct sockaddr_in conn_addr = {
      .sin_family = AF_INET,
      .sin_port = port,
      .sin_addr = addr
  };

  int sock_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
  //ERROR_HANDLE(sock_fd, ERROR_CODE, "socket")

  SendPing(sock_fd, (struct sockaddr*) &conn_addr, sizeof(conn_addr), timeout, interval);

  close(sock_fd);
  return 0;
}
