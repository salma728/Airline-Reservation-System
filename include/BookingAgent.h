#ifndef BOOKINGAGENT_H
#define BOOKINGAGENT_H

#include "User.h"
#include <memory>
#include <string>

using namespace std;

class Reservation;

class BookingAgent : public User
{
public:
    BookingAgent(int id, const string &username, const string &password);
    shared_ptr<Reservation> createReservation(int passengerId, const string &flightNumber);
    void assistCheckIn(int reservationId);
    void cancelReservation(int reservationId);
    void viewMenu() const override;
};

#endif