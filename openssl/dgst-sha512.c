#include <openssl/sha.h>
#include <stdio.h>
#include <unistd.h>
#include <values.h>

int main() {
  const int HASH_SIZE = 512 / CHAR_BIT;
  SHA512_CTX context;
  SHA512_Init(&context);

  char buffer[HASH_SIZE];
  ssize_t bytes_read;
  while ((bytes_read = read(0, buffer, sizeof(buffer))) > 0) {
    SHA512_Update(&context, buffer, bytes_read);
  }
  unsigned char hash[HASH_SIZE];
  SHA512_Final(hash, &context);

  printf("0x");
  for (size_t i = 0; i < sizeof(hash); i++) {
    printf("%02"PRIx32, hash[i]&0xffU);
  }

  printf("\n");

  return 0;
}