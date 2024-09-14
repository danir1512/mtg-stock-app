#include <string>
#include <fstream> 
#include <iostream> 
#include <sstream>
#include <vector>
#include <memory>

struct Card {
    std::string name;
    int value;
};

using Collection = std::vector<Card>;

class User {
    public:

        User(const std::string userName, const std::string password);

        int addCard(const std::string card_name, int price);
        int removeCard(const std::string card);
        auto getCollection() const -> const Collection&;
        auto getUserName() const -> const std::string { return m_userName; }

        // Load library
        void loadCardsFromTxtFile(const std::string& fileName);

    private:
        std::shared_ptr<Collection> m_collection;
        std::string m_userName;
        std::string m_password;
};