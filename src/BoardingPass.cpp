#include "BoardingPass.h"
#include <sstream>

using namespace std;

BoardingPass::BoardingPass(int bpId, int reservationId, const string &gate, const string &seat, const string boardingTime)
    : bpId(bpId), reservationId(reservationId), gate(gate), seat(seat), boardingTime(boardingTime) {}

string BoardingPass::generatePrintable() const
{
    ostringstream os;
    os << "Reservation ID: " << reservationId << "\nSeat: " << seat << "\nGate: " << gate << "\nBoarding Time: " << boardingTime;
    return os.str();
}