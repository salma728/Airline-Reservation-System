#include "Flight.h"
#include <iostream>
#include <cctype>

using namespace std;
using json = nlohmann::json;

static int seatColsDefault = 6; // seats per row A-F

Flight::Flight(const string &flightNumber, const string &origin, const string &destination, const string &departureTime, const string &arrivalTime, shared_ptr<Aircraft> aircraft)
    : flightNumber(flightNumber), origin(origin), destination(destination), departureTime(departureTime), arrivalTime(arrivalTime), status(FlightStatus::OnTime), aircraft(aircraft)
{
    int total = aircraft ? aircraft->getTotalSeats() : 0;
    cols = seatColsDefault;
    rows = (cols > 0) ? (total / cols) : 1;
    if (rows <= 0)
        rows = 1;
    seatMap.resize(rows);
    for (int r = 0; r < rows; ++r)
    {
        seatMap[r].resize(cols);
        for (int c = 0; c < cols; ++c)
        {
            char colLetter = 'A' + c;
            seatMap[r][c] = make_shared<Seat>(r + 1, colLetter, SeatClass::Economy);
        }
    }
}

bool Flight::isSeatAvailable(int row, char col) const
{
    if (row < 1 || row > rows)
        return false;
    int ci = toupper(static_cast<unsigned char>(col)) - 'A';
    if (ci < 0 || ci >= cols)
        return false;
    return seatMap[row - 1][ci]->available();
}

bool Flight::reserveSeat(int row, char col, int reservationId)
{
    if (!isSeatAvailable(row, col))
        return false;
    int ci = toupper(static_cast<unsigned char>(col)) - 'A';
    seatMap[row - 1][ci]->reserve(reservationId);
    return true;
}

bool Flight::cancelSeat(int row, char col)
{
    if (row < 1 || row > rows)
        return false;
    int ci = toupper(static_cast<unsigned char>(col)) - 'A';
    if (ci < 0 || ci >= cols)
        return false;
    seatMap[row - 1][ci]->free();
    return true;
}

void Flight::updateStatus(FlightStatus newStatus)
{
    status = newStatus;
}
int Flight::getAvailableSeats() const
{
    int k = 0;
    for (auto &r : seatMap)
        for (auto &s : r)
            if (s->available())
                ++k;
    return k;
}
string Flight::getFlightNumber() const
{
    return flightNumber;
}

string Flight::getOrigin() const { return origin; }
string Flight::getDestination() const { return destination; }
string Flight::getDepartureTime() const { return departureTime; }
string Flight::getArrivalTime() const { return arrivalTime; }
FlightStatus Flight::getStatus() const { return status; }
shared_ptr<Aircraft> Flight::getAircraft() const { return aircraft; }

json Flight::exportSeatMapForPersistence() const
{
    json arr = json::array();
    for (int r = 0; r < rows; ++r)
    {
        for (int c = 0; c < cols; ++c)
        {
            auto s = seatMap[r][c];
            json js;
            js["row"] = r + 1;
            js["col"] = string(1, s->getCol());
            js["occupied"] = !s->available();
            js["reservationId"] = s->getReservationId();
            arr.push_back(js);
        }
    }
    return arr;
}

void Flight::importSeatMapFromPersistence(const json &jseats)
{
    if (!jseats.is_array())
        return;
    for (auto &js : jseats)
    {
        int row = js.value("row", 0);
        string colS = js.value("col", string("A"));
        char col = colS.empty() ? 'A' : colS[0];
        bool occupied = js.value("occupied", false);
        int reservationId = js.value("reservationId", 0);
        if (row >= 1 && row <= rows)
        {
            int ci = toupper(static_cast<unsigned char>(col)) - 'A';
            if (ci >= 0 && ci < cols)
            {
                if (occupied)
                {
                    seatMap[row - 1][ci]->reserve(reservationId);
                }
                else
                {
                    seatMap[row - 1][ci]->free();
                }
            }
        }
    }
}