#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <cctype>

using namespace std;

// trim and parse seat label helpers
inline string trim(const string &s)
{
    size_t a = s.find_first_not_of(" \t\n\r");
    if (a == string::npos)
        return "";
    size_t b = s.find_last_not_of(" \t\n\r");
    return s.substr(a, b - a + 1);
}

inline bool parseSeatLabel(const string &label, int &row, char &col)
{
    if (label.size() < 2)
        return false;
    size_t i = 0;
    while (i < label.size() && isdigit(static_cast<unsigned char>(label[i])))
        ++i;
    if (i == 0)
        return false;
    string rowPart = label.substr(0, i);
    string colPart = label.substr(i);
    if (colPart.size() != 1)
        return false;
    try
    {
        row = stoi(rowPart);
    }
    catch (...)
    {
        return false;
    }
    col = colPart[0];
    return true;
}

#endif