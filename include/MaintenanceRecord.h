#ifndef MAINTENANCERECORD_H
#define MAINTENANCERECORD_H

#include <string>

using namespace std;

class MaintenanceRecord
{
private:
    int recordId;
    string date;
    string description;
    string performedBy;

public:
    MaintenanceRecord() = default;
    MaintenanceRecord(int recordId, const string &date, const string &description, const string &performedBy);
    string getSummary() const;
};

#endif