#include "User.h"
#include <cctype>
#include "Security.h"
using namespace std;

User::User(int id, const string &username, const string &password, Role role)
    : id(id), username(username), role(role)
{
    setPassword(password);
}

int User::getId() const { return id; }
string User::getUsername() const { return username; }
Role User::getRole() const { return role; }
bool User::login(const string &pw) const { return sha256(salt + pw) == passwordHash; }
void User::setPassword(const string &pw)
{
    salt = generateSalt();
    passwordHash = sha256(salt + pw);
}

string User::getPasswordHash() const { return passwordHash; }
string User::getSalt() const { return salt; }
void User::setPasswordHashAndSalt(const string &hash, const string &saltVal)
{
    passwordHash = hash;
    salt = saltVal;
}

bool User::isValidPassword(const string &pw)
{
    if (pw.size() < 8)
        return false;
    bool up = false, lo = false, dig = false;
    for (char c : pw)
    {
        if (isupper(static_cast<unsigned char>(c)))
            up = true;
        else if (islower(static_cast<unsigned char>(c)))
            lo = true;
        else if (isdigit(static_cast<unsigned char>(c)))
            dig = true;
    }
    return up && lo && dig;
}
