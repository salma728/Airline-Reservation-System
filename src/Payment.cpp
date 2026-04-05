#include "Payment.h"
#include <iostream>
using namespace std;

Payment::Payment(int paymentId, int reservationId, double amount, PaymentMethod method)
    : paymentId(paymentId), reservationId(reservationId), amount(amount), method(method), transactionRef(""), succeeded(false) {}

bool Payment::processPayment()
{
    succeeded = true;
    transactionRef = "TXN" + to_string(paymentId);
    // cout << "[Payment] processed payment " << transactionRef << " amount=" << amount << "\n";
    return true;
}
bool Payment::refund()
{
    if (!succeeded)
        return false;
    succeeded = false;
    cout << "[Payment] refunded paymentId=" << paymentId << "\n";
    return true;
}
int Payment::getReservationId() const { return reservationId; }
int Payment::getPaymentId() const { return paymentId; }
double Payment::getAmount() const { return amount; }
PaymentMethod Payment::getMethod() const { return method; }
bool Payment::isSucceeded() const { return succeeded; }
string Payment::getTransactionRef() const { return transactionRef; }
void Payment::setTransactionRef(const string &tx) { transactionRef = tx; }