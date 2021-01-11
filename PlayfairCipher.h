#include <vector>
#include <string>

class PlayfairCipher{
    enum class Mode{
        Encrypt,
        Decrypt
    };

    public:
        PlayfairCipher(const std::string& message, const std::string& keyword);

        std::string Encrypt() const;
        std::string Decrypt() const;

        void SetMessage(std::string message);
        void PrintPolybiusSquare() const;
    private:
        std::vector<std::pair<char, char>> m_Message;
        std::string m_FormattedKeyword;
        std::vector<char> m_PolybiusSquare;

        void InitializeMessage(const std::string& message);
        void InitializePolybiusSquare(const std::string& keyword);
        std::string Run(Mode mode) const;

        int GetRow(int index) const;
        int GetColumn(int index) const;
        int GetIndex(int row, int column) const;
};