#ifndef SECURITY_H
#define SECURITY_H

#include <string>

using namespace std;

// Provides minimal hashing utilities for passwords.
// Uses SHA-256 with a per-user random salt.
string sha256(const string &input);
string generateSalt(size_t length = 16);

#endif