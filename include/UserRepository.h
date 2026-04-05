#ifndef USERREPOSITORY_H
#define USERREPOSITORY_H

#include <map>
#include <memory>
#include <string>
#include <vector>
#include "User.h"

using namespace std;

class UserRepository
{
private:
    map<int, shared_ptr<User>> users;

public:
    UserRepository() = default;
    shared_ptr<User> findById(int id) const;
    shared_ptr<User> findByUsername(const string &username) const;
    void addUser(shared_ptr<User> u);
    void removeUser(int id);
    vector<shared_ptr<User>> listAll() const;
};

#endif