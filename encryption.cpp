#include "encryption.h"
#include <cctype>

bool performCaesarCipher(string& content, bool encrypt) {
    int shift = encrypt ? 3 : -3;

    for (char& ch : content) {
        if (isalpha(ch)) {
            char base = isupper(ch) ? 'A' : 'a';  
            ch = static_cast<char>((ch - base + shift + 26) % 26 + base);
        }
    }

    return true;
}

bool encryptString(string& text, bool encrypt) {
    if (performCaesarCipher(text, encrypt)) {
        return true;
    }
    return false;
}
