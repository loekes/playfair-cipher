#include <iostream>

#include "PlayfairCipher.h"

int main() {
    std::string key{"fail"};
    std::string phrase{"All DAE students will get twenties all accross the board!"};
    PlayfairCipher p{phrase, key};

    std::string encrypted{p.Encrypt()};
    p.SetMessage(encrypted);
    std::string decrypted(p.Decrypt());

    std::cout << "--- Playfair Cipher ---\n\n";
    std::cout << "Phrase:\t\t\t" << phrase << "\n";
    std::cout << "Key:\t\t\t" << key << "\n";
    std::cout << "Encrypted:\t\t" << encrypted << "\n";
    std::cout << "Decrypted:\t\t" << decrypted << "\n";
}