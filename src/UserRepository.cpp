#include "UserRepository.h"
#include <algorithm>

using namespace std;

shared_ptr<User> UserRepository::findById(int id) const
{
    auto it = users.find(id);
    if (it == users.end())
        return nullptr;
    return it->second;
}
shared_ptr<User> UserRepository::findByUsername(const string &username) const
{
    for (auto &p : users)
        if (p.second->getUsername() == username)
            return p.second;
    return nullptr;
}
void UserRepository::addUser(shared_ptr<User> u)
{
    if (!u)
        return;
    users[u->getId()] = u;
}
void UserRepository::removeUser(int id) { users.erase(id); }
vector<shared_ptr<User>> UserRepository::listAll() const
{
    vector<shared_ptr<User>> v;
    for (auto &p : users)
        v.push_back(p.second);
    return v;
}