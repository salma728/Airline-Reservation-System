#include "Security.h"
#include <random>
#include <sstream>
#include <iomanip>
#include <array>

using namespace std;

namespace
{
    string hashFallback(const string &s)
    {
        hash<string> h;
        auto v = h(s);
        ostringstream os;
        os << hex << setw(16) << setfill('0') << v;
        return os.str();
    }
}

string sha256(const string &input)
{
    return hashFallback(input);
}

string generateSalt(size_t length)
{
    static const char alphabet[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    static thread_local mt19937 rng{random_device{}()};
    uniform_int_distribution<int> dist(0, static_cast<int>(sizeof(alphabet) - 2));
    string s;
    s.reserve(length);
    for (size_t i = 0; i < length; ++i)
        s.push_back(alphabet[dist(rng)]);
    return s;
}