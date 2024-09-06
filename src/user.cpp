#include "user.hpp"

#include <algorithm>

User::User(const std::string userName, const std::string password) : m_userName(userName), m_password(password) {
    m_collection = std::make_shared<Collection>();
    addCard("Teferi, Hero of Dominaria");
    addCard("Nicol Bolas, the Ravager");
    addCard("Nissa, Who Shakes the World");
    addCard("Chandra, Awakened Inferno");
    addCard("Sorin, Imperious Bloodlord");
    addCard("Ajani, Strength of the Pride");
    addCard("Mu Yanling, Sky Dancer");
};

int User::addCard(const std::string card) {
    //TODO: Cjeck if cards exist. Add if it does, otherwise return error.
    m_collection->push_back(card);
    return 0;
}

int User::removeCard(const std::string card) {
    const auto iterCardIndex{std::find_if(m_collection->begin(), m_collection->end(), [card](const auto cardName) {
        return cardName == card;
    })};

    if(iterCardIndex != m_collection->end()) {
        m_collection->erase(iterCardIndex);
        return 0;
    }
}

auto User::getCollection() const -> const Collection& {
    return *m_collection;
}
