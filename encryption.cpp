 #include "encryption.h"
 #include <fstream>
 #include <cctype>

 using namespace std;

bool performCaesarCipher(string& content, bool encrypt){
     
}

 bool encryptFile(const string& filename, bool encrypt)
 {
     // Open the input file
     ifstream inFile(filename);
     if (!inFile) {
         cout << "Cannot open file: " << filename << endl;
         return false;
     }
     
     // Read the contents of the file
     string content(istreambuf_iterator<char>(inFile), {});
     inFile.close();

     // Encrypt or decrypt the content
    if (performCaesarCipher(content, encrypt)){

    }

     
 }
  