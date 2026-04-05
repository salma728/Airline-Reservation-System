#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include <string>
#include <vector>
#include "MaintenanceRecord.h"

using namespace std;

class Aircraft
{
private:
    string regNumber;
    string model;
    int totalSeats;
    vector<MaintenanceRecord> maintenanceLogs;

public:
    Aircraft() = default;
    Aircraft(const string &regNumber, const string &model, int totalSeats);
    bool needsMaintenance() const;
    void addMaintenanceLog(const MaintenanceRecord &rec);
    string getModel() const;
    int getTotalSeats() const;
    string getRegNumber() const;
};

#endif