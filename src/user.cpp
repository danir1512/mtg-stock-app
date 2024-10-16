#include "user.hpp"
#include "dbg.h"
#include <algorithm>

User::User(const std::string userName, const std::string password) : m_userName(userName), m_password(password) {
    m_collection = std::make_shared<Collection>();
};

int User::addCard(const std::string card_name, CardTypes type ,int price) {
    //TODO: Cjeck if cards exist. Add if it does, otherwise return error.
    m_collection->emplace_back(Card{card_name, type, price});
    return 0;
}

int User::removeCard(const std::string card_name) {
    const auto iterCardIndex{std::find_if(m_collection->begin(), m_collection->end(), [card_name](const auto cardName) {
        return cardName.m_name == card_name;
    })};

    if(iterCardIndex != m_collection->end()) {
        m_collection->erase(iterCardIndex);
        return 0;
    }
}

void User::loadCardsFromTxtFile(const std::string& fileName) {
    std::ifstream inputFile(fileName);

    if(!inputFile.is_open()) {
        //TODO: Add logging display popup
        std::cerr << "Error: Could not open file: " << fileName << std::endl;
        return;
    }

    std::string line, cardName, cardAmount;
    while (getline(inputFile, line)) {

        // Skip empty line from the sideboard
        if(std::isspace(line[0])) {
            continue;
        }

        std::istringstream iss(line);

        iss >> cardAmount;
        std::getline(iss, cardName, '\n');

        cardName = cardName.substr(1); // remove leading space
        addCard(cardName, CardTypes::CREATURE ,std::stoi(cardAmount));
    }

    inputFile.close();
}

void User::saveCollectionToTxtFile(const std::string& fileName) const {

    auto new_file_name = fileName;

    if (!(new_file_name.rfind(".txt") == (new_file_name.size() - std::string(".txt").size()))) {
        dbg("File extension is not .txt!!!! Adding .txt to the end of the file name");
        new_file_name += ".txt";
    }

    std::ofstream outputFile(new_file_name);

    if(!outputFile.is_open()) {
        std::cerr << "Error: Could not open file: " << new_file_name << std::endl;
    }

    for(const auto& card : *m_collection) {
        outputFile << card.m_value << ";" << card.m_name << std::endl;
    }
    
    outputFile.close();
}

void User::createNewDeck(const std::string deckName, const std::string format) {

    m_decks.push_back(std::make_shared<Deck>(deckName, format));
}

void Deck::addNewCardsToDeck(const std::string cardName, CardTypes cardType, int cardAmount) {

    auto cardIter = std::find_if(cards.begin(), cards.end(), [cardName](const auto card) {
        return card.m_name == cardName;
    });

    if (cardIter != cards.end()) {
        cardIter->m_value++;
    } else {
        cards.emplace_back(Card{cardName, cardType ,cardAmount});
        
    }
}

void Deck::displayDeck(const std::string deck_name) const {
    for(const auto& card : cards) {
        dbg(card.m_name);
    }
}