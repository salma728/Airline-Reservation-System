#include "Reservation.h"

using namespace std;

Reservation::Reservation(int reservationId, int passengerId, const string &flightNumber, int seatRow, char seatCol, double price)
    : reservationId(reservationId), passengerId(passengerId), flightNumber(flightNumber), seatRow(seatRow), seatCol(seatCol), status("Confirmed"), price(price), checkedIn(false) {}

void Reservation::confirm()
{
    status = "Confirmed";
}
void Reservation::cancel()
{
    status = "Cancelled";
}
bool Reservation::isActive() const
{
    return status == "Confirmed";
}
bool Reservation::isCheckedIn() const
{
    return checkedIn;
}
void Reservation::markCheckedIn(const string &g, const string &bt)
{
    checkedIn = true;
    gate = g;
    boardingTime = bt;
}
int Reservation::getId() const
{
    return reservationId;
}
int Reservation::getPassengerId() const
{
    return passengerId;
}
string Reservation::getFlightNumber() const
{
    return flightNumber;
}
int Reservation::getSeatRow() const
{
    return seatRow;
}
char Reservation::getSeatCol() const
{
    return seatCol;
}
string Reservation::getStatus() const
{
    return status;
}
double Reservation::getPrice() const
{
    return price;
}
string Reservation::getGate() const
{
    return gate;
}
string Reservation::getBoardingTime() const
{
    return boardingTime;
}