#include <iostream>
#include "encryption.h"

using namespace std;

int main()
{
    char mode;
    string text;
    
    cout << "Enter the text: ";
    getline(cin >> ws, text);
    
    cout << "Encrypt (e) or Decrypt (d)? "; 
    cin >> mode;
    
    if (mode == 'e' || mode == 'E') {
        string encryptedText = text; // Placeholder for encryption
        if (encryptString(encryptedText, true)) {
            cout << "Encrypted text: " << encryptedText << endl;
        } else {
            cerr << "Error encrypting text." << endl;
        }
    } else if (mode == 'd' || mode == 'D') {
        string decryptedText = text; // Placeholder for decryption
        if (encryptString(decryptedText, false)) {
            cout << "Decrypted text: " << decryptedText << endl;
        } else {
            cerr << "Error decrypting text." << endl;
        }
    } else {
        cerr << "Error: Invalid mode. Use 'e' for encryption or 'd' for decryption." << endl;
    }
    
    return 0;
}
