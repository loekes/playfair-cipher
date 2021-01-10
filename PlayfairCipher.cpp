#include "PlayfairCipher.h"

#include <iostream>
#include <numeric>
#include <algorithm>

PlayfairCipher::PlayfairCipher(const std::string& message, const std::string& keyword)
{
    InitializeMessage(message);
    InitializePolybiusSquare(keyword);
}

std::string PlayfairCipher::Encrypt() const {
    return Run(Mode::Encrypt);
}

std::string PlayfairCipher::Decrypt() const {
    return Run(Mode::Decrypt);
}

std::string PlayfairCipher::Run(Mode mode) const {
    std::string result{};

    std::for_each(m_Message.begin(), m_Message.end(), [&](const std::pair<char, char>& p) {
        //calculate indices
        auto result1{std::find(m_PolybiusSquare.begin(), m_PolybiusSquare.end(), p.first)};
        int index1{int(std::distance(m_PolybiusSquare.begin(), result1))};

        auto result2{std::find(m_PolybiusSquare.begin(), m_PolybiusSquare.end(), p.second)};
        int index2{int(std::distance(m_PolybiusSquare.begin(), result2))};
        
        int row1{GetRow(index1)};
        int row2{GetRow(index2)};
        int column1{GetColumn(index1)};
        int column2{GetColumn(index2)};
        
        if(row1 == row2) {
            //if rows are equal
            if(mode == Mode::Encrypt) {
                result.push_back(m_PolybiusSquare.at(GetIndex(row1, (column1 == 5) ? 1 : column1 + 1)));
                result.push_back(m_PolybiusSquare.at(GetIndex(row2, (column2 == 5) ? 1 : column2 + 1)));
            }
            else if(mode == Mode::Decrypt) {
                result.push_back(m_PolybiusSquare.at(GetIndex(row1, (column1 == 1) ? 5 : column1 - 1)));
                result.push_back(m_PolybiusSquare.at(GetIndex(row2, (column2 == 1) ? 5 : column2 - 1)));
            }
            
        } else if(column1 == column2) {
            //if columns are equal
            if(mode == Mode::Encrypt) {
                result.push_back(m_PolybiusSquare.at(GetIndex((row1 == 5) ? 1 : row1 + 1, column1)));
                result.push_back(m_PolybiusSquare.at(GetIndex((row2 == 5) ? 1 : row2 + 1, column2)));
            } else if(mode == Mode::Decrypt) {
                result.push_back(m_PolybiusSquare.at(GetIndex((row1 == 1) ? 5 : row1 - 1, column1)));
                result.push_back(m_PolybiusSquare.at(GetIndex((row2 == 1) ? 5 : row2 - 1, column2)));
            }
        } else {
            //if columns and rows are not equal
            result.push_back(m_PolybiusSquare.at(GetIndex(row1, column2)));
            result.push_back(m_PolybiusSquare.at(GetIndex(row2, column1)));
        }
    });

    return result;
}

void PlayfairCipher::SetMessage(std::string message) {
    m_Message.clear();
    InitializeMessage(message);
}

void PlayfairCipher::InitializeMessage(const std::string& message) {
    std::string tmpMessage{message};
    //to lower
    std::transform(tmpMessage.begin(), tmpMessage.end(), tmpMessage.begin(), [](char c){ 
            return std::tolower(c); 
    });
    //remove non alpha characters
    tmpMessage.erase(std::remove_if(tmpMessage.begin(), tmpMessage.end(), [](char c) {
        return !(std::isalpha(c));
    }), tmpMessage.end());

    for(size_t i{}; i < tmpMessage.size(); ++i) {
        if(i < tmpMessage.size() - 1) {
            if(tmpMessage[i] == tmpMessage[i + 1]) {
                char c1 = tmpMessage[i];
                m_Message.push_back({c1, 'x'});
            } else {
                char c1 = tmpMessage[i];
                char c2 = tmpMessage[i + 1];
                m_Message.push_back({c1, c2});
                ++i;
            }
        } else {
            char c1 = tmpMessage[i];
            m_Message.push_back({c1, 'x'});
        }
    }
}

void PlayfairCipher::InitializePolybiusSquare(const std::string& keyword) {
    //remove double letters
    std::string formattedKeyword{};
    std::for_each(keyword.begin(), keyword.end(), [&formattedKeyword](char c){ 
        if(std::find(formattedKeyword.begin(), formattedKeyword.end(), c) == formattedKeyword.end()) {
            formattedKeyword.push_back(c);
        }
    }); 

    //intialize square
    m_PolybiusSquare = std::vector<char>(formattedKeyword.size());
    //only insert unique characters into formatted keyword
    std::unique_copy(formattedKeyword.begin(), formattedKeyword.end(), m_PolybiusSquare.begin());

    std::vector<char> alphabet(26);
    //generate an alphabet
    std::iota(alphabet.begin(), alphabet.end(), int('a'));
    //remove j and y (i represents i, j and y)
    alphabet.erase(std::remove(alphabet.begin(), alphabet.end(), 'j'), alphabet.end());

    std::for_each(alphabet.begin(), alphabet.end(), [&](char c) {
        if(std::find(m_PolybiusSquare.begin(), m_PolybiusSquare.end(), c) == m_PolybiusSquare.end()) {
            //not yet in there
            m_PolybiusSquare.push_back(c);
        }
    });
}

int PlayfairCipher::GetRow(int index) const {
    return index / 5 + 1;
}

int PlayfairCipher::GetColumn(int index) const {
    return index % 5 + 1;
}

int PlayfairCipher::GetIndex(int row, int column) const {
    return ((row - 1) * 5) + (column - 1);
}