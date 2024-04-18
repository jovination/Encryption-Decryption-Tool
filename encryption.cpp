#include "encryption.h"
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#include <fstream>
#include <iostream>

using namespace std;

const int AES_BLOCK_SIZE = 16;
const int MAX_IV_LEN = 16;
unsigned char key[] = "0123456789abcdef";

void generateRandomIV(unsigned char *iv) {
    RAND_bytes(iv, MAX_IV_LEN);
}

void handleErrors() {
    ERR_print_errors_fp(stderr);
}

void aes_encrypt(const string& plaintext, unsigned char* ciphertext, unsigned char* iv, unsigned char* key){
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        handleErrors();
        return;
    }

    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), nullptr, key, iv)) {
        handleErrors();
        EVP_CIPHER_CTX_free(ctx);
        return;
    }

    int len;
    if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, reinterpret_cast<const unsigned char*>(plaintext.data()), plaintext.size())) {
        handleErrors();
        EVP_CIPHER_CTX_free(ctx);
        return;
    }
    int ciphertext_len = len;

    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) {
        handleErrors();
        EVP_CIPHER_CTX_free(ctx);
        return;
    }
    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);
}

void aes_decrypt(const unsigned char* ciphertext, unsigned char* decryptedtext, int ciphertext_len, unsigned char* iv, unsigned char* key) {
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        handleErrors();
        return;
    }

    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), nullptr, key, iv)) {
        handleErrors();
        EVP_CIPHER_CTX_free(ctx);
        return;
    }

    int len;
    if (1 != EVP_DecryptUpdate(ctx, decryptedtext, &len, ciphertext, ciphertext_len)) {
        handleErrors();
        EVP_CIPHER_CTX_free(ctx);
        return;
    }
    int plaintext_len = len;

    if (1 != EVP_DecryptFinal_ex(ctx, decryptedtext + len, &len)) {
        handleErrors();
        EVP_CIPHER_CTX_free(ctx);
        return;
    }
    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);
}

bool encryptFile(const std::string& filename, bool encrypt, unsigned char* key) {
    std::ifstream inFile(filename, std::ios::binary);
    if (!inFile) {
        std::cout << "Cannot open file: " << filename << std::endl;
        return false;
    }

    std::string content((std::istreambuf_iterator<char>(inFile)), {});
    inFile.close();

    unsigned char iv[MAX_IV_LEN];
    generateRandomIV(iv);

    if (encrypt) {
        unsigned char ciphertext[content.size() + AES_BLOCK_SIZE];
        aes_encrypt(content, ciphertext, iv, key);

        std::ofstream outFile("encrypted_" + filename, std::ios::binary);
        if (!outFile) {
            std::cout << "Cannot create output file." << std::endl;
            return false;
        }
        outFile.write(reinterpret_cast<const char*>(iv), MAX_IV_LEN);
        outFile.write(reinterpret_cast<const char*>(ciphertext), content.size() + AES_BLOCK_SIZE);
        outFile.close();
    } 
    
    else {
        std::ifstream inFileEncrypted("encrypted_" + filename, std::ios::binary);
        if (!inFileEncrypted) {
            std::cout << "Cannot open encrypted file." << std::endl;
            return false;
        }

        unsigned char storedIV[MAX_IV_LEN];
        inFileEncrypted.read(reinterpret_cast<char*>(storedIV), MAX_IV_LEN);

        inFileEncrypted.seekg(0, std::ios::end);
        int ciphertext_size = static_cast<int>(inFileEncrypted.tellg()) - MAX_IV_LEN;
        inFileEncrypted.seekg(MAX_IV_LEN, std::ios::beg);

        unsigned char* ciphertext = new unsigned char[ciphertext_size];
        inFileEncrypted.read(reinterpret_cast<char*>(ciphertext), ciphertext_size);
        inFileEncrypted.close();

        unsigned char* decryptedtext = new unsigned char[ciphertext_size + AES_BLOCK_SIZE];
        aes_decrypt(ciphertext, decryptedtext, ciphertext_size, storedIV, key);

        std::ofstream outFile("decrypted_" + filename, std::ios::binary);
        if (!outFile) {
            std::cout << "Cannot create output file." << std::endl;
            delete[] ciphertext;
            delete[] decryptedtext;
            return false;
        }
        outFile.write(reinterpret_cast<const char*>(decryptedtext), ciphertext_size);
        outFile.close();

        delete[] ciphertext;
        delete[] decryptedtext;
    }

    return true;
}

bool decryptFile(const std::string& filename, unsigned char* key) {
    std::ifstream inFileEncrypted("encrypted_" + filename, std::ios::binary);
    if (!inFileEncrypted) {
        std::cout << "Cannot open encrypted file." << std::endl;
        return false;
    }

    unsigned char storedIV[MAX_IV_LEN];
    inFileEncrypted.read(reinterpret_cast<char*>(storedIV), MAX_IV_LEN);

    inFileEncrypted.seekg(0, std::ios::end);
    int ciphertext_size = static_cast<int>(inFileEncrypted.tellg()) - MAX_IV_LEN;
    inFileEncrypted.seekg(MAX_IV_LEN, std::ios::beg);

    unsigned char* ciphertext = new unsigned char[ciphertext_size];
    inFileEncrypted.read(reinterpret_cast<char*>(ciphertext), ciphertext_size);
    inFileEncrypted.close();

    unsigned char* decryptedtext = new unsigned char[ciphertext_size + AES_BLOCK_SIZE];
    aes_decrypt(ciphertext, decryptedtext, ciphertext_size, storedIV, key);

    std::ofstream outFile("decrypted_" + filename, std::ios::binary);
    if (!outFile) {
        std::cout << "Cannot create output file." << std::endl;
        delete[] ciphertext;
        delete[] decryptedtext;
        return false;
    }
    outFile.write(reinterpret_cast<const char*>(decryptedtext), ciphertext_size);
    outFile.close();

    delete[] ciphertext;
    delete[] decryptedtext;

    return true;
}
