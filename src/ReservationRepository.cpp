#include "ReservationRepository.h"

using namespace std;

shared_ptr<Reservation> ReservationRepository::findById(int id) const
{
    auto it = reservations.find(id);
    if (it == reservations.end())
        return nullptr;
    return it->second;
}
void ReservationRepository::addReservation(shared_ptr<Reservation> r)
{
    if (!r)
        return;
    reservations[r->getId()] = r;
}
void ReservationRepository::removeReservation(int id) { reservations.erase(id); }
vector<shared_ptr<Reservation>> ReservationRepository::findByPassengerId(int passengerId) const
{
    vector<shared_ptr<Reservation>> v;
    for (auto &p : reservations)
        if (p.second->getPassengerId() == passengerId)
            v.push_back(p.second);
    return v;
}
vector<shared_ptr<Reservation>> ReservationRepository::listAll() const
{
    vector<shared_ptr<Reservation>> v;
    for (auto &p : reservations)
        v.push_back(p.second);
    return v;
}