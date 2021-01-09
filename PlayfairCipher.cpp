#include "PlayfairCipher.h"

#include <iostream>
#include <numeric>
#include <algorithm>

PlayfairCipher::PlayfairCipher(std::string message, std::string keyword)
{
    InitializeMessage(message);
    InitializePolybiusSquare(keyword);
}

std::string PlayfairCipher::Encrypt() {
    std::string result{};

    std::for_each(m_Message.begin(), m_Message.end(), [=, &result](std::pair<char, char>& p) {
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
            result.push_back(m_PolybiusSquare.at(GetIndex(row1, (column1 == 5) ? 1 : column1 + 1)));
            result.push_back(m_PolybiusSquare.at(GetIndex(row2, (column2 == 5) ? 1 : column2 + 1)));
        } else if(column1 == column2) {
            //if columns are equal
            result.push_back(m_PolybiusSquare.at(GetIndex((row1 == 5) ? 1 : row1 + 1, column1)));
            result.push_back(m_PolybiusSquare.at(GetIndex((row2 == 5) ? 1 : row2 + 1, column2)));
        } else {
            //if columns and rows are not equal
            result.push_back(m_PolybiusSquare.at(GetIndex(row1, column2)));
            result.push_back(m_PolybiusSquare.at(GetIndex(row2, column1)));
        }
    });

    return result;
}

std::string PlayfairCipher::Decrypt() {
    std::string result{};

    std::for_each(m_Message.begin(), m_Message.end(), [=, &result](std::pair<char, char>& p) {
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
            result.push_back(m_PolybiusSquare.at(GetIndex(row1, (column1 == 1) ? 5 : column1 - 1)));
            result.push_back(m_PolybiusSquare.at(GetIndex(row2, (column2 == 1) ? 5 : column2 - 1)));
        } else if(column1 == column2) {
            //if columns are equal
            result.push_back(m_PolybiusSquare.at(GetIndex((row1 == 1) ? 5 : row1 - 1, column1)));
            result.push_back(m_PolybiusSquare.at(GetIndex((row2 == 1) ? 5 : row2 - 1, column2)));
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

void PlayfairCipher::InitializeMessage(std::string message) {
    //to lower
    std::transform(message.begin(), message.end(), message.begin(), [](char c){ 
            return std::tolower(c); 
    });
    //remove non alpha characters
    message.erase(std::remove_if(message.begin(), message.end(), [](char c) {
        return !(std::isalpha(c));
    }), message.end());

    for(size_t i{}; i < message.size(); ++i) {
        if(i < message.size() - 1) {
            if(message[i] == message[i + 1]) {
                char c1 = message[i];
                m_Message.push_back({c1, 'x'});
            } else {
                char c1 = message[i];
                char c2 = message[i + 1];
                m_Message.push_back({c1, c2});
                ++i;
            }
        } else {
            char c1 = message[i];
            m_Message.push_back({c1, 'x'});
        }
    }
}

void PlayfairCipher::InitializePolybiusSquare(std::string keyword) {
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

    std::for_each(alphabet.begin(), alphabet.end(), [=](char c) {
        if(std::find(m_PolybiusSquare.begin(), m_PolybiusSquare.end(), c) == m_PolybiusSquare.end()) {
            //not yet in there
            m_PolybiusSquare.push_back(c);
        }
    });
}

int PlayfairCipher::GetRow(int index) {
    return index / 5 + 1;
}

int PlayfairCipher::GetColumn(int index) {
    return index % 5 + 1;
}

int PlayfairCipher::GetIndex(int row, int column) {
    return ((row -1) * 5) + (column - 1);
}

