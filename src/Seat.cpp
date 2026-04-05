#include "Seat.h"
#include <sstream>
using namespace std;

Seat::Seat(int row, char col, SeatClass sc) : row(row), col(col), isOccupied(false), seatClass(sc), reservationId(0) {}
bool Seat::available() const
{
    return !isOccupied;
}
void Seat::reserve(int rid)
{
    isOccupied = true;
    reservationId = rid;
}
void Seat::free()
{
    isOccupied = false;
    reservationId = 0;
}
string Seat::getLabel() const
{
    ostringstream os;
    os << row << col;
    return os.str();
}
int Seat::getReservationId() const
{
    return reservationId;
}
char Seat::getCol() const
{
    return col;
}
int Seat::getRow() const
{
    return row;
}