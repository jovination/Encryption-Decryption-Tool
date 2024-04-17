#include "encryption.h"
#include <openssl/evp.h>
#include <openssl/err.h>  // Include this
#include <fstream>
#include <iostream>
#include <cctype>


// AES key for encryption and decryption (128-bit key)
unsigned char key[] = "0123456789abcdef";

// Handle errors
void handleErrors(void)
{
    ERR_print_errors_fp(stderr);
    abort();
}

// AES key for encryption function
void aes_encrypt(const std::string& plaintext, unsigned char* ciphertext) {
    EVP_CIPHER_CTX *ctx;
    int len;
    int ciphertext_len;

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

    if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, NULL))
        handleErrors();

    if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, reinterpret_cast<const unsigned char*>(plaintext.c_str()), plaintext.size()))
        handleErrors();
    ciphertext_len = len;

    if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) handleErrors();
    ciphertext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);
}

/// AES key for decryption function
void aes_decrypt(const unsigned char* ciphertext, unsigned char* decryptedtext, int ciphertext_len) {
    EVP_CIPHER_CTX *ctx;
    int len;
    int plaintext_len;

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

    if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, NULL))
        handleErrors();

    if(1 != EVP_DecryptUpdate(ctx, decryptedtext, &len, ciphertext, ciphertext_len))
        handleErrors();
    plaintext_len = len;

    if(1 != EVP_DecryptFinal_ex(ctx, decryptedtext + len, &len)) handleErrors();
    plaintext_len += len;

    /* Clean up */
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

    // Perform AES encryption or decryption
    if (encrypt) {
        // Encrypt the content
        unsigned char ciphertext[content.size()];
        aes_encrypt(content, ciphertext);

        // Write the encrypted content to a new file
        std::ofstream outFile("encrypted_" + filename, std::ios::binary);
        if (!outFile) {
            std::cout << "Cannot create output file." << std::endl;
            return false;
        }
        outFile.write(reinterpret_cast<const char*>(ciphertext), sizeof(ciphertext));
    } 
    
    else {
        // Decrypt the content
        unsigned char decryptedtext[content.size()];
        aes_decrypt(reinterpret_cast<const unsigned char*>(content.c_str()), decryptedtext, content.size());

        // Write the decrypted content to a new file
        std::ofstream outFile("decrypted_" + filename, std::ios::binary);
        if (!outFile) {
            std::cout << "Cannot create output file." << std::endl;
            return false;
        }
        outFile.write(reinterpret_cast<const char*>(decryptedtext), sizeof(decryptedtext));
    }

    return true;
}
