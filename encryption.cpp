#include "encryption.h"
#include <fstream>
#include <iostream> // Added for cout
#include <cctype>

using namespace std;

bool performCaesarCipher(string& content, bool encrypt) {
    int shift = encrypt ? 3 : -3; // Removed extra spaces around the ternary operator

    for (char& ch : content) {
        if (isalpha(ch)) {
            char base = isupper(ch) ? 'A' : 'a';  
            ch = static_cast<char>((ch - base + shift + 26) % 26 + base); // Corrected stastic_cast to static_cast
        }
    }

    return true; // Added return statement, replace with appropriate logic
}

bool encryptFile(const string& filename, bool encrypt) {
    // Open the input file
    ifstream inFile(filename);
    if (!inFile) {
        cout << "Cannot open file: " << filename << endl;
        return false;
    }
    
    // Read the contents of the file
    string content((istreambuf_iterator<char>(inFile)), {}); // Corrected initialization of string content
    inFile.close();

    // Encrypt or decrypt the content
    if (performCaesarCipher(content, encrypt)) {
        // Create an output file and write the modified content
        ofstream outFile((encrypt ? "encrypted_" : "") + filename); // Corrected creation of output file name
        if (!outFile) {
            cout << "Cannot create output file." << endl;
            return false;
        }

        outFile << content;
        outFile.close();
        
        return true;
    }

    return false; // Return false if encryption/decryption fails
}
