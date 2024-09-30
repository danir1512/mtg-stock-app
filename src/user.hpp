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

class Deck {
    public:
        Deck(const std::string deckName, const std::string format) : name(deckName), format(format) {}
    


    private:
        std::string name;
        std::string format;
        std::vector<Card> cards;
};

using Collection = std::vector<Card>;

class User {
    public:

        // Default constructor
        User() = default;
 
        // Constructor
        User(const std::string userName, const std::string password);
        
        // Get reference to User card collection
        auto getCollection() -> Collection& { return *m_collection; }

        // Get const reference to User card collection
        auto getCollection() const -> const Collection& { return *m_collection; }

        // Get user password
        auto getPassword() const -> const std::string { return m_password; }

        // Get user name
        auto getUserName() const -> const std::string { return m_userName; }

        // Set user name
        void setUserName(const std::string userName) { m_userName = userName; }

        //Set user password
        void setPassword(const std::string password) { m_password = password; }

        // Add card to user collection
        int addCard(const std::string card_name, int price);

        // Remove card from user collection
        int removeCard(const std::string card);

        // Load library
        // Add cards to user collection from txt file.
        void loadCardsFromTxtFile(const std::string& fileName);

        // Save library
        void saveCollectionToTxtFile(const std::string& filename) const;

        // Create a new deck
        void createNewDeck(const std::string deckName, const std::string format);

        // Void Add new card to deck
        void addNewCardsToDeck(const std::string deckName, const std::string cardName, int cardAmount);

    private:
        std::string m_userName;
        std::string m_password;
        std::shared_ptr<Collection> m_collection;
        std::vector<std::shared_ptr<Deck>> m_decks;
       
};