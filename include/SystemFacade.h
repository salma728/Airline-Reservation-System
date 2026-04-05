#ifndef SYSTEMFACADE_H
#define SYSTEMFACADE_H

#include <memory>
#include "User.h"

using namespace std;

class SystemFacade
{
public:
    SystemFacade() = default;
    shared_ptr<User> authenticate(const string &username, const string &password);
    void start();
};

#endif