 #include "encryption.h"
 #include <fstream>
 #include <cctype>

 using namespace std;

bool performCaesarCipher(string& content, bool encrypt){
   int shift = encrypt ? 3   : -3 ;

   for (char& ch: content) {
    if(isalpha(ch)){
       char base = isupper(ch) ? 'A' : 'a';  
       ch = stastic_cast<char>((ch - base + shift + 26) % 26 + base) ;  
       
    /*  
        ch += shift;
        if(ch > 'z'){
            ch -= 26;
        }
        else if(ch < 'a'){
            ch += 26;
        }
  */
    }
    
  }

  return true;

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
  