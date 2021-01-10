#include <iostream>

#include "PlayfairCipher.h"

int main() {
    std::string key{"death"};
    std::string message{"LABOULAYE LADY WILL LEAD TO CIBOLA TEMPLES OF GOLD"};
    PlayfairCipher p{message, key};

    std::string encrypted{p.Encrypt()};
    p.SetMessage(encrypted);
    std::string decrypted(p.Decrypt());

    std::cout << "--- Playfair Cipher ---\n\n";
    std::cout << "Message:\t\t" << message << "\n";
    std::cout << "Key:\t\t\t" << key << "\n";
    std::cout << "Encrypted:\t\t" << encrypted << "\n";
    std::cout << "Decrypted:\t\t" << decrypted << "\n";
}