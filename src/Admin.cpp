#include "Admin.h"
#include "DataManager.h"
#include "Aircraft.h"
#include "Flight.h"
#include "IdGenerator.h"
#include <iostream>

using namespace std;

Admin::Admin(int id, const string &username, const string &password)
    : User(id, username, password, Role::Admin) {}

void Admin::createUser(shared_ptr<User> user)
{
    if (!user)
    {
        cout << "[Admin] null user\n";
        return;
    }
    auto dm = DataManager::getInstance();
    dm->userRepo->addUser(user);
    dm->saveAll("data");
    cout << "[Admin] Created user: " << user->getUsername() << "\n";
}

void Admin::deleteUser(int userId)
{
    auto dm = DataManager::getInstance();
    dm->userRepo->removeUser(userId);
    dm->saveAll("data");
    cout << "[Admin] delete user id=" << userId << "\n";
}

void Admin::manageFlight(const string &flightNumber)
{
    auto dm = DataManager::getInstance();
    cout << "[Admin] manage flight " << flightNumber << "\n";
}

void Admin::viewReports()
{
    auto dm = DataManager::getInstance();
    auto flights = dm->flightRepo->listAll();
    int total = static_cast<int>(flights.size());
    cout << "[Admin] Operational Report (demo)\nTotal Flights: " << total << "\n";
}

void Admin::viewMenu() const
{
    cout << "\n=== Administrator Menu (" << getUsername() << ") ===\n";
    cout << "1. Manage Flights\n2. Manage Aircraft\n3. Manage Users\n4. Generate Reports\n5. Logout\n";
}