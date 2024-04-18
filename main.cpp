#include <iostream>
#include "encryption.h"

using namespace std;

int main() {
    string filename;
    char mode;

    cout << "Enter the filename: ";
    getline(cin >> ws, filename);

    cout << "Encrypt (e) or Decrypt (d)? ";
    cin >> mode;

    if (mode == 'e' || mode == 'E') {
        if (encryptFile(filename, true, key)) {
            cout << "File encrypted successfully." << endl;
        } else {
            cerr << "Error encrypting file." << endl;
        }
    } else if (mode == 'd' || mode == 'D') {
        if (encryptFile(filename, false, key)) {
            cout << "File decrypted successfully." << endl;
        } else {
            cerr << "Error decrypting file." << endl;
        }
    } else {
        cerr << "Error: Invalid mode. Use 'e' for encryption or 'd' for decryption." << endl;
    }
    return 0;
}
