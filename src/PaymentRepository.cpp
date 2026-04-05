#include "PaymentRepository.h"
using namespace std;

void PaymentRepository::addPayment(shared_ptr<Payment> p)
{
    if (!p)
        return;
    payments[p->getReservationId()] = p;
}
shared_ptr<Payment> PaymentRepository::findByReservationId(int reservationId) const
{
    auto it = payments.find(reservationId);
    if (it == payments.end())
        return nullptr;
    return it->second;
}
shared_ptr<Payment> PaymentRepository::findById(int paymentId) const
{
    for (auto &p : payments)
        if (p.second && p.second->getPaymentId() == paymentId)
            return p.second;
    return nullptr;
}
vector<shared_ptr<Payment>> PaymentRepository::listAll() const
{
    vector<shared_ptr<Payment>> v;
    for (auto &p : payments)
        v.push_back(p.second);
    return v;
}
