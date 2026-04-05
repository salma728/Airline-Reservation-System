#ifndef SEAT_H
#define SEAT_H

#include <string>
#include "SeatClass.h"

using namespace std;

class Seat
{
private:
    int row;
    char col;
    bool isOccupied;
    SeatClass seatClass;
    int reservationId;

public:
    Seat(int row, char col, SeatClass sc);
    bool available() const;
    void reserve(int reservationId);
    void free();
    string getLabel() const;
    int getReservationId() const;
    char getCol() const;
    int getRow() const;
};

#endif