#ifndef USER_H
#define USER_H

#include <string>
#include "Role.h"

using namespace std;

class User
{
private:
    string passwordHash;
    string salt;

protected:
    int id;
    string username;
    Role role;

public:
    User(int id, const string &username, const string &password, Role role);
    virtual ~User() = default;
    int getId() const;
    string getUsername() const;
    Role getRole() const;
    virtual bool login(const string &pw) const;
    void setPassword(const string &pw);
    string getPasswordHash() const;
    string getSalt() const;
    void setPasswordHashAndSalt(const string &hash, const string &saltVal);
    static bool isValidPassword(const string &pw);
    virtual void viewMenu() const = 0;
};

#endif