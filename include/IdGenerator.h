#ifndef IDGENERATOR_H
#define IDGENERATOR_H

class IdGenerator
{
protected:
    static int nextID;

public:
    static int getNextId();
    static void reset(int start = 1);
};

#endif
