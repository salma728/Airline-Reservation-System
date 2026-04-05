#ifndef RESERVATIONREPOSITORY_H
#define RESERVATIONREPOSITORY_H

#include <map>
#include <memory>
#include <vector>
#include "Reservation.h"

using namespace std;

class ReservationRepository
{
private:
    map<int, shared_ptr<Reservation>> reservations;

public:
    ReservationRepository() = default;
    shared_ptr<Reservation> findById(int id) const;
    void addReservation(shared_ptr<Reservation> r);
    void removeReservation(int id);
    vector<shared_ptr<Reservation>> findByPassengerId(int passengerId) const;
    vector<shared_ptr<Reservation>> listAll() const;
};

#endif