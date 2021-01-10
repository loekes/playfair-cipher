#include "PlayfairCipher.h"

#include <iostream>
#include <numeric>
#include <algorithm>

PlayfairCipher::PlayfairCipher(const std::string& message, const std::string& keyword)
{
    InitializeMessage(message);
    InitializePolybiusSquare(keyword);
}

/// <summary>Encrypts the current message based on the Polybius Square</summary>
/// <returns>Returns the encrypted message</returns>
std::string PlayfairCipher::Encrypt() const {
    return Run(Mode::Encrypt);
}

/// <summary>Decrypts the current message based on the Polybius Square</summary>
/// <returns>Returns the decrypted message</returns>
std::string PlayfairCipher::Decrypt() const {
    return Run(Mode::Decrypt);
}

/// <summary>Encrypts/Decrypts the current message based on the Polybius Square</summary>
/// <param name="mode">the mode to run with (encrypt/decrypt)</param>
/// <returns>Returns the encrypted/decrypted message</returns>
std::string PlayfairCipher::Run(Mode mode) const {
    std::string result{};

    std::for_each(m_Message.begin(), m_Message.end(), [&](const std::pair<char, char>& p) {
        //find index of the pair's first character 
        auto result1{std::find(m_PolybiusSquare.begin(), m_PolybiusSquare.end(), p.first)};
        int index1{int(std::distance(m_PolybiusSquare.begin(), result1))};

        //find index of the pair's second character 
        auto result2{std::find(m_PolybiusSquare.begin(), m_PolybiusSquare.end(), p.second)};
        int index2{int(std::distance(m_PolybiusSquare.begin(), result2))};
        
        //convert indices to rows and columns on the Polybius Square
        int row1{GetRow(index1)};
        int row2{GetRow(index2)};
        int column1{GetColumn(index1)};
        int column2{GetColumn(index2)};
        
        if(row1 == row2) {
            //1) if rows are equal
            if(mode == Mode::Encrypt) {
                //shift right and wrap around when encrypting
                result.push_back(m_PolybiusSquare.at(GetIndex(row1, (column1 == 5) ? 1 : column1 + 1)));
                result.push_back(m_PolybiusSquare.at(GetIndex(row2, (column2 == 5) ? 1 : column2 + 1)));
            }
            else if(mode == Mode::Decrypt) {
                //shift left and wrap around when decrypting
                result.push_back(m_PolybiusSquare.at(GetIndex(row1, (column1 == 1) ? 5 : column1 - 1)));
                result.push_back(m_PolybiusSquare.at(GetIndex(row2, (column2 == 1) ? 5 : column2 - 1)));
            }
            
        } else if(column1 == column2) {
            //2) if rows are not equal but columns are
            if(mode == Mode::Encrypt) {
                //shift down and wrap around when encrypting
                result.push_back(m_PolybiusSquare.at(GetIndex((row1 == 5) ? 1 : row1 + 1, column1)));
                result.push_back(m_PolybiusSquare.at(GetIndex((row2 == 5) ? 1 : row2 + 1, column2)));
            } else if(mode == Mode::Decrypt) {
                //shift up and wrap around when decrypting
                result.push_back(m_PolybiusSquare.at(GetIndex((row1 == 1) ? 5 : row1 - 1, column1)));
                result.push_back(m_PolybiusSquare.at(GetIndex((row2 == 1) ? 5 : row2 - 1, column2)));
            }
        } else {
            //3) if rows and columns are not equal
            //form a "square" with both characters and use the other corners to encrypt and decrypt
            result.push_back(m_PolybiusSquare.at(GetIndex(row1, column2)));
            result.push_back(m_PolybiusSquare.at(GetIndex(row2, column1)));
        }
    });

    return result;
}

/// <summary>Changes the message to encrypt/decrypt</summary>
/// <param name="message">the message - either a phrase or cipher</param>
void PlayfairCipher::SetMessage(std::string message) {
    //clear current message
    m_Message.clear();
    //initialize message again
    InitializeMessage(message);
}

/// <summary>Initializes the message (convert to lower case, remove non-alpha characters) by splitting it up into pairs (digrams)</summary>
/// <param name="message">the original message</param>
void PlayfairCipher::InitializeMessage(const std::string& message) {
    std::string tmpMessage{message};
    
    //convert to lower case
    std::transform(tmpMessage.begin(), tmpMessage.end(), tmpMessage.begin(), [](char c){ 
            return std::tolower(c); 
    });
    //remove non-alpha characters
    tmpMessage.erase(std::remove_if(tmpMessage.begin(), tmpMessage.end(), [](char c) {
        return !(std::isalpha(c));
    }), tmpMessage.end());

    for(size_t i{}; i < tmpMessage.size(); ++i) {
        if(i < tmpMessage.size() - 1) {
            //if not last character
            if(tmpMessage[i] == tmpMessage[i + 1]) {
                //if this and next character are equal
                //create pair of this character and x
                char c1 = tmpMessage[i];
                m_Message.push_back({c1, 'x'});
            } else {
                //if this and next character are not equal
                //create pair of this and next character
                char c1 = tmpMessage[i];
                char c2 = tmpMessage[i + 1];
                m_Message.push_back({c1, c2});
                //shift one additional space to skip next character
                ++i;
            }
        } else {
            //if last character (and uneven in this case)
            //create pair of last character and 'x'
            char c1 = tmpMessage[i];
            m_Message.push_back({c1, 'x'});
        }
    }
}


/// <summary>Initializes the 5x5 Polybius Square based on the keyword</summary>
/// <param name="keyword">the keyword used to encrypt/decrypt</param>
void PlayfairCipher::InitializePolybiusSquare(const std::string& keyword) {
    //remove double letters from keyword
    std::string formattedKeyword{};
    std::for_each(keyword.begin(), keyword.end(), [&formattedKeyword](char c){ 
        if(std::find(formattedKeyword.begin(), formattedKeyword.end(), c) == formattedKeyword.end()) {
            formattedKeyword.push_back(c);
        }
    }); 

    //intialize square with the size of the formatted keyword
    m_PolybiusSquare = std::vector<char>(formattedKeyword.size());
    //copy characters into square
    std::copy(formattedKeyword.begin(), formattedKeyword.end(), m_PolybiusSquare.begin());

    //generate a full alphabet
    std::vector<char> alphabet(26);
    std::iota(alphabet.begin(), alphabet.end(), int('a'));
    //remove j (i and j are considered the same character)
    alphabet.erase(std::remove(alphabet.begin(), alphabet.end(), 'j'), alphabet.end());

    //insert all characters from the alphabet into square if they are not yet in there
    std::for_each(alphabet.begin(), alphabet.end(), [&](char c) {
        if(std::find(m_PolybiusSquare.begin(), m_PolybiusSquare.end(), c) == m_PolybiusSquare.end()) {
            m_PolybiusSquare.push_back(c);
        }
    });
}

/// <summary>Converts a vector index to a row number on the 5x5 Polybius Square</summary>
/// <param name="index">index of the character in the Polybius Square vector</param>
/// <returns>Returns the row number</returns>
int PlayfairCipher::GetRow(int index) const {
    return index / 5 + 1;
}

/// <summary>Converts a vector index to a column number on the 5x5 Polybius Square</summary>
/// <param name="index">index of the character in the Polybius Square vector</param>
/// <returns>Returns the column number</returns>
int PlayfairCipher::GetColumn(int index) const {
    return index % 5 + 1;
}

/// <summary>Converts a row and column on the 5x5 Polybius Square to a vector index</summary>
/// <param name="row">row of the character on the Polybius Square</param>
/// <param name="column">column of the character on the Polybius Square</param>
/// <returns>Returns the index</returns>
int PlayfairCipher::GetIndex(int row, int column) const {
    return ((row - 1) * 5) + (column - 1);
}