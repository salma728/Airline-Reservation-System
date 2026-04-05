#include "IdGenerator.h"
using namespace std;

int IdGenerator::nextID = 1;

int IdGenerator::getNextId()
{
    return nextID++;
}

void IdGenerator::reset(int start)
{
    nextID = start;
}
