#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <memory>
#include <string>
#include "UserRepository.h"
#include "FlightRepository.h"
#include "ReservationRepository.h"
#include "PaymentRepository.h"

using namespace std;

class DataManager
{
private:
    static DataManager *instance;
    DataManager();

public:
    unique_ptr<UserRepository> userRepo;
    unique_ptr<FlightRepository> flightRepo;
    unique_ptr<ReservationRepository> reservationRepo;
    unique_ptr<PaymentRepository> paymentRepo;
    static DataManager *getInstance();
    bool loadAll(const string &path = "data");
    bool saveAll(const string &path = "data");
};

#endif