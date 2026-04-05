#ifndef PAYMENTREPOSITORY_H
#define PAYMENTREPOSITORY_H

#include <map>
#include <memory>
#include "Payment.h"
#include <vector>

using namespace std;

class PaymentRepository
{
private:
    map<int, shared_ptr<Payment>> payments;

public:
    PaymentRepository() = default;
    void addPayment(shared_ptr<Payment> p);
    shared_ptr<Payment> findByReservationId(int reservationId) const;
    shared_ptr<Payment> findById(int paymentId) const;
    vector<shared_ptr<Payment>> listAll() const;
};

#endif