#include "encryption.h"
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#include <fstream>
#include <iostream>

const int AES_BLOCK_SIZE = 16; // AES block size in bytes
const int MAX_IV_LEN = 16; // Maximum length of IV for AES

// Function to generate a random Initialization Vector (IV)
void generateRandomIV(unsigned char *iv) {
    RAND_bytes(iv, MAX_IV_LEN);
}
// Handle errors
void handleErrors(void)
{
    ERR_print_errors_fp(stderr);
    abort();
}


// AES key for encryption function
void aes_encrypt(const std::string& plaintext, unsigned char* ciphertext, unsigned char* iv) {
    EVP_CIPHER_CTX *ctx;
    int len;
    int ciphertext_len;

    if(!(ctx = EVP_CIPHER_CTX_new())) {
        handleErrors();
    }

    if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv)) {
        handleErrors();
    }

    if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, reinterpret_cast<const unsigned char*>(plaintext.c_str()), plaintext.size())) {
        handleErrors();
    }
    ciphertext_len = len;

    if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) {
        handleErrors();
    }
    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);
}

// AES key for decryption function
void aes_decrypt(const unsigned char* ciphertext, unsigned char* decryptedtext, int ciphertext_len, unsigned char* iv) {
    EVP_CIPHER_CTX *ctx;
    int len;
    int plaintext_len;

    if(!(ctx = EVP_CIPHER_CTX_new())) {
        handleErrors();
    }

    if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv)) {
        handleErrors();
    }

    if(1 != EVP_DecryptUpdate(ctx, decryptedtext, &len, ciphertext, ciphertext_len)) {
        handleErrors();
    }
    plaintext_len = len;

    if(1 != EVP_DecryptFinal_ex(ctx, decryptedtext + len, &len)) {
        handleErrors();
    }
    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);
}

// Function to encrypt or decrypt a file using AES
bool encryptFile(const std::string& filename, bool encrypt) {
    // Open the input file
    std::ifstream inFile(filename, std::ios::binary);
    if (!inFile) {
        std::cout << "Cannot open file: " << filename << std::endl;
        return false;
    }

    // Read the contents of the file
    std::string content((std::istreambuf_iterator<char>(inFile)), {});
    inFile.close();

    // Generate a random IV
    unsigned char iv[MAX_IV_LEN];
    generateRandomIV(iv);

    // Perform AES encryption or decryption
    if (encrypt) {
        // Encrypt the content
        unsigned char ciphertext[content.size() + AES_BLOCK_SIZE]; // Add space for padding
        aes_encrypt(content, ciphertext, iv);

        // Write the IV and encrypted content to a new file
        std::ofstream outFile("encrypted_" + filename, std::ios::binary);
        if (!outFile) {
            std::cout << "Cannot create output file." << std::endl;
            return false;
        }
        outFile.write(reinterpret_cast<const char*>(iv), MAX_IV_LEN);
        outFile.write(reinterpret_cast<const char*>(ciphertext), content.size());
        outFile.close();
    } 
    else {
        // Extract IV from the file
        unsigned char storedIV[MAX_IV_LEN];
        inFile.read(reinterpret_cast<char*>(storedIV), MAX_IV_LEN);

        // Decrypt the content
        unsigned char decryptedtext[content.size()]; // Should be large enough
        aes_decrypt(reinterpret_cast<const unsigned char*>(content.c_str()), decryptedtext, content.size(), storedIV);

        // Write the decrypted content to a new file
        std::ofstream outFile("decrypted_" + filename, std::ios::binary);
        if (!outFile) {
            std::cout << "Cannot create output file." << std::endl;
            return false;
        }
        outFile.write(reinterpret_cast<const char*>(decryptedtext), content.size());
        outFile.close();
    }

    return true;
}
