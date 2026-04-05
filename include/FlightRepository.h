#ifndef FLIGHTREPOSITORY_H
#define FLIGHTREPOSITORY_H

#include <map>
#include <memory>
#include <string>
#include <vector>
#include "Flight.h"

using namespace std;

class FlightRepository
{
private:
    map<string, shared_ptr<Flight>> flights;

public:
    FlightRepository() = default;
    shared_ptr<Flight> findByNumber(const string &flightNumber) const;
    void addFlight(shared_ptr<Flight> f);
    void removeFlight(const string &flightNumber);
    vector<shared_ptr<Flight>> listAll() const;
};

#endif