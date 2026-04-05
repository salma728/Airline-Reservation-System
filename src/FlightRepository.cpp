#include "FlightRepository.h"

using namespace std;

shared_ptr<Flight> FlightRepository::findByNumber(const string &flightNumber) const
{
    auto it = flights.find(flightNumber);
    if (it == flights.end())
        return nullptr;
    return it->second;
}
void FlightRepository::addFlight(shared_ptr<Flight> f)
{
    if (!f)
        return;
    flights[f->getFlightNumber()] = f;
}
void FlightRepository::removeFlight(const string &flightNumber)
{
    flights.erase(flightNumber);
}
vector<shared_ptr<Flight>> FlightRepository::listAll() const
{
    vector<shared_ptr<Flight>> v;
    for (auto &p : flights)
        v.push_back(p.second);
    return v;
}