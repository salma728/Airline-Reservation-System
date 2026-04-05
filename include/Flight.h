#ifndef FLIGHT_H
#define FLIGHT_H

#include <string>
#include <vector>
#include <memory>
#include "FlightStatus.h"
#include "Aircraft.h"
#include "Seat.h"
#include "nlohmann/json.hpp"

using namespace std;

class Reservation;

class Flight
{
private:
    string flightNumber;
    string origin;
    string destination;
    string departureTime;
    string arrivalTime;
    FlightStatus status;
    shared_ptr<Aircraft> aircraft;
    vector<vector<shared_ptr<Seat>>> seatMap; // rows x cols
    int rows;
    int cols;

public:
    Flight(const string &flightNumber,
           const string &origin,
           const string &destination,
           const string &departureTime,
           const string &arrivalTime,
           shared_ptr<Aircraft> aircraft);
    bool isSeatAvailable(int row, char col) const;
    bool reserveSeat(int row, char col, int reservationId);
    bool cancelSeat(int row, char col);
    void updateStatus(FlightStatus newStatus);
    int getAvailableSeats() const;
    string getFlightNumber() const;

    string getOrigin() const;
    string getDestination() const;
    string getDepartureTime() const;
    string getArrivalTime() const;
    FlightStatus getStatus() const;
    shared_ptr<Aircraft> getAircraft() const;

    nlohmann::json exportSeatMapForPersistence() const;
    void importSeatMapFromPersistence(const nlohmann::json &jseats);
};

#endif