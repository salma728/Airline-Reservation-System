#include "MaintenanceRecord.h"
#include <sstream>

using namespace std;

MaintenanceRecord::MaintenanceRecord(int recordId, const string &date, const string &description, const string &performedBy)
    : recordId(recordId), date(date), description(description), performedBy(performedBy) {}

string MaintenanceRecord::getSummary() const
{
    ostringstream os;
    os << date << " - " << description << " by " << performedBy;
    return os.str();
}
