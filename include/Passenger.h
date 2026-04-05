#ifndef PASSENGER_H
#define PASSENGER_H

#include "User.h"

#include <memory>
#include <string>
#include "PaymentMethod.h"

using namespace std;

class Reservation;

class Passenger : public User
{
private:
    string name;
    string contact;
    int loyaltyPoints;

public:
    Passenger(int id, const string &username, const string &password,
              const string &name, const string &contact);
    string getName() const;
    string getContact() const;
    int getLoyaltyPoints() const;
    void addLoyaltyPoints(int points);
    void setLoyaltyPoints(int points);
    void searchFlights(const string &origin, const string &destination, const string &date) const;
    shared_ptr<Reservation> bookFlight(const string &flightNumber, int row, char col, PaymentMethod method, const string &methodStr);
    void checkIn(int reservationId);
    void viewMenu() const override;
    void viewReservations() const;
};

#endif