#ifndef ADMIN_H
#define ADMIN_H

#include "User.h"
#include <memory>
#include <string>

using namespace std;

class Admin : public User
{
public:
    Admin(int id, const string &username, const string &password);
    void createUser(shared_ptr<User> user);
    void deleteUser(int userId);
    void manageFlight(const string &flightNumber);
    void viewReports();
    void viewMenu() const override;
};

#endif
