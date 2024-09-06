#include <string>
#include <vector>
#include <memory>

using Collection = std::vector<std::string>;

class User {
    public:

        User(const std::string userName, const std::string password);

        int addCard(const std::string card);
        int removeCard(const std::string card);
        auto getCollection() const -> const Collection&;
        auto getUserName() const -> const std::string { return m_userName; }

    private:
        std::shared_ptr<Collection> m_collection;
        std::string m_userName;
        std::string m_password;
};