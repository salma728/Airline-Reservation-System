#ifndef PAYMENT_H
#define PAYMENT_H

#include <string>
#include "PaymentMethod.h"

using namespace std;

class Payment
{
private:
    int paymentId;
    int reservationId;
    double amount;
    PaymentMethod method;
    string transactionRef;
    bool succeeded;
    string details;

public:
    Payment(int paymentId, int reservationId, double amount, PaymentMethod method);

    void setDetails(const string &d)
    {
        details = d;
    }
    string getDetails() const
    {
        return details;
    }
    bool processPayment();
    bool refund();
    int getReservationId() const;
    int getPaymentId() const;
    double getAmount() const;
    PaymentMethod getMethod() const;
    bool isSucceeded() const;
    string getTransactionRef() const;
    void setTransactionRef(const string &tx);
};

#endif