#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <string>

using namespace std;

// Declare the AES encryption function
void aes_encrypt(const string& plaintext, unsigned char* ciphertext);

// Function to encrypt or decrypt a file using AES
bool encryptFile(const string& filename, bool encrypt);

#endif // ENCRYPTION_H