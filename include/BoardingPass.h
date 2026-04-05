#ifndef BOARDINGPASS_H
#define BOARDINGPASS_H

#include <string>

using namespace std;

class BoardingPass
{
private:
    int bpId;
    int reservationId;
    string gate;
    string seat;
    string boardingTime;

public:
    BoardingPass(int bpId, int reservationId, const string &gate, const string &seat, const string boardingTime);
    string generatePrintable() const;
};

#endif