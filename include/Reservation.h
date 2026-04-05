#ifndef RESERVATION_H
#define RESERVATION_H

#include <string>

using namespace std;

class Reservation
{
private:
    int reservationId;
    int passengerId;
    string flightNumber;
    int seatRow;
    char seatCol;
    string status;
    double price;
    bool checkedIn;
    string gate;
    string boardingTime;

public:
    Reservation(int reservationId, int passengerId, const string &flightNumber, int seatRow, char seatCol, double price);
    void confirm();
    void cancel();
    bool isActive() const;
    bool isCheckedIn() const;
    void markCheckedIn(const string &gate, const string &boardingTime);
    int getId() const;
    int getPassengerId() const;
    string getFlightNumber() const;
    int getSeatRow() const;
    char getSeatCol() const;
    string getStatus() const;
    double getPrice() const;
    string getGate() const;
    string getBoardingTime() const;
};

#endif