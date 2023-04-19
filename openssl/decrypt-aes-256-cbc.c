#include <openssl/evp.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char* argv[]) {
  if (argc < 2) {
    return 1;
  }

  uint8_t* password = (uint8_t*) argv[1];
  size_t password_length = strlen(argv[1]);

  uint8_t salt[8];
  uint8_t buffer[4096];

  read(0, buffer, 16);

  // salt 8-15
  for (size_t i = 0; i < 8; i++) {
    salt[i] = buffer[i + 8];
  }

  EVP_CIPHER_CTX* context = EVP_CIPHER_CTX_new();
  uint8_t key[32];
  uint8_t iv[16];

  EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha256(), salt, password, password_length, 1, key, iv);

  EVP_DecryptInit(context, EVP_aes_256_cbc(), key, iv);

  ssize_t bytes_read;
  uint8_t answer[4096];
  int answer_len = 0;

  while ((bytes_read = read(0, buffer, sizeof(buffer))) > 0) {
    EVP_DecryptUpdate(context, answer, &answer_len, buffer, bytes_read);
    write(1, answer, answer_len);
  }

  EVP_DecryptFinal(context, answer, &answer_len);
  write(1, answer, answer_len);

  EVP_CIPHER_CTX_free(context);

  return 0;
}