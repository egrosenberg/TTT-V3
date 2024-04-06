#ifndef TTT_COG_H
#define TTT_COG_H

#include "TTTengine.h"
#include <map>
#include <functional>
#include <type_traits>

class TTTcog;

class TTTcog
{
private:
    int c_ID;
public:
    TTTcog();

    int ID() { return c_ID; }

    virtual ~TTTcog();
};


#endif