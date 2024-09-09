#include "user.hpp"

#include <algorithm>

User::User(const std::string userName, const std::string password) : m_userName(userName), m_password(password) {
    m_collection = std::make_shared<Collection>();
    addCard("Teferi, Hero of Dominaria", 20);
    addCard("Nicol Bolas, the Ravager", 45);
    addCard("Nissa, Who Shakes the World", 70);
    addCard("Chandra, Awakened Inferno", 80);
    addCard("Sorin, Imperious Bloodlord", 98);
    addCard("Ajani, Strength of the Pride", 90);
    addCard("Mu Yanling, Sky Dancer", 80);
};

int User::addCard(const std::string card_name, int price) {
    //TODO: Cjeck if cards exist. Add if it does, otherwise return error.
    m_collection->emplace_back(Card{card_name, price});
    return 0;
}

int User::removeCard(const std::string card_name) {
    const auto iterCardIndex{std::find_if(m_collection->begin(), m_collection->end(), [card_name](const auto cardName) {
        return cardName.name == card_name;
    })};

    if(iterCardIndex != m_collection->end()) {
        m_collection->erase(iterCardIndex);
        return 0;
    }
}

auto User::getCollection() const -> const Collection& {
    return *m_collection;
}
