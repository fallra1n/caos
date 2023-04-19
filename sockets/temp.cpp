#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#define DNS_SERVER "8.8.8.8"
#define DNS_PORT 53
#define MAX_HOSTNAME_LEN 256
#define MAX_DNS_RESPONSE_LEN 1024

int main() {
  char hostname[MAX_HOSTNAME_LEN];

  while (scanf("%s", hostname) == 1) {
    // Создаем UDP-сокет
    int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_fd == -1) {
      perror("socket");
      continue;
    }

    // Разбираем IP-адрес DNS-сервера
    struct sockaddr_in dns_addr;
    memset(&dns_addr, 0, sizeof(dns_addr));
    dns_addr.sin_family = AF_INET;
    dns_addr.sin_port = htons(DNS_PORT);
    if (inet_pton(AF_INET, DNS_SERVER, &dns_addr.sin_addr) != 1) {
      perror("inet_pton");
      continue;
    }

    // Формируем DNS-запрос в формате, понятном серверу
    char dns_query[MAX_HOSTNAME_LEN + 18];
    int query_len = snprintf(dns_query, sizeof(dns_query),
                             "\x00\x00"  // Идентификатор запроса
                             "\x01\x00"  // Флаги: единственный запрос, рекурсивный поиск
                             "\x00\x01"  // Количество вопросов
                             "\x00\x00"  // Количество ответов
                             "\x00\x00"  // Количество NS-записей
                             "\x00\x00"  // Количество дополнительных записей
                             "%s"       // Имя хоста
                             "\x00"      // Конец имени
                             "\x00\x01"  // Тип запроса: A
                             "\x00\x01", // Класс запроса: IN
                             hostname);

    // Отправляем запрос на DNS-сервер
    if (sendto(sock_fd, dns_query, query_len, 0,
               (struct sockaddr*)&dns_addr, sizeof(dns_addr)) == -1) {
      perror("sendto");
      continue;
    }

    // Читаем ответ от DNS-сервера
    char dns_response[MAX_DNS_RESPONSE_LEN];
    struct sockaddr_in response_addr;
    socklen_t response_addr_len = sizeof(response_addr);
    int response_len = recvfrom(sock_fd, dns_response, sizeof(dns_response), 0,
                                (struct sockaddr*)&response_addr, &response_addr_len);
    if (response_len == -1) {
      perror("recvfrom");
      continue;
    }

    // Парсим ответ и выводим IP-адрес
    int answer_count = (dns_response[6] << 8) | dns_response[7];
    if (answer_count == 0) {
      printf("No answer for %s\n", hostname);
      continue;
    }
    int answer_offset = 12 + strlen(hostname) + 2 + 4; // Смещение ответа в буфере
    printf("%d.%d.%d.%d\n", (unsigned char)dns_response[answer_offset + 0],
           (unsigned char)dns_response[answer_offset + 1],
           (unsigned char)dns_response[answer_offset + 2],
           (unsigned char)dns_response[answer_offset + 3]);

    // Закрываем сокет
    fflush(stdout);
    close(sock_fd);
  }

  return 0;
}