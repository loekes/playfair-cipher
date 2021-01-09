#include <vector>
#include <string>
#include <unordered_set>

class PlayfairCipher{
    public:
        PlayfairCipher(std::string message, std::string keyword);
        std::string Encrypt();
        std::string Decrypt();
        void SetMessage(std::string message);
    private:
        std::vector<std::pair<char, char>> m_Message;
        std::vector<char> m_PolybiusSquare;

        void InitializeMessage(std::string message);
        void InitializePolybiusSquare(std::string keyword);

        int GetRow(int index);
        int GetColumn(int index);
        int GetIndex(int row, int column);
};