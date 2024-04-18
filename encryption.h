// encryption.h
#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <string>

extern unsigned char key[];

// Function declarations
void aes_encrypt(const std::string& plaintext, unsigned char* ciphertext, unsigned char* iv, unsigned char* key);
void aes_decrypt(const unsigned char* ciphertext, unsigned char* decryptedtext, int ciphertext_len, unsigned char* iv, unsigned char* key);
bool encryptFile(const std::string& filename, bool encrypt, unsigned char* key);

#endif // ENCRYPTION_H
