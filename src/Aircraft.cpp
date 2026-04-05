#include "Aircraft.h"
using namespace std;

Aircraft::Aircraft(const string &regNumber, const string &model, int totalSeats)
    : regNumber(regNumber), model(model), totalSeats(totalSeats) {}

bool Aircraft::needsMaintenance() const
{
    return maintenanceLogs.size() > 0;
}
void Aircraft::addMaintenanceLog(const MaintenanceRecord &rec)
{
    maintenanceLogs.push_back(rec);
}
string Aircraft::getModel() const
{
    return model;
}
int Aircraft::getTotalSeats() const
{
    return totalSeats;
}
string Aircraft::getRegNumber() const
{
    return regNumber;
}