#ifndef TTT_ENGINE_H
#define TTT_ENGINE_H

#include <vector>
#include <utility>
#include <typeinfo>
#include <iostream>
#include "TTT.h"
#include "TTTcog.h"
#include "Terminal.h"

class TTTcog;

class TTTengine
{
private:
    TTTengine();
    static TTTengine *m_singleton;

    unsigned int m_CID;
    std::vector<TTTcog*> *m_cogs;

public:
    static TTTengine *GetSingleton();

    TTTengine(TTTengine const&)      = delete;
    void operator=(TTTengine const&) = delete;

    int BuildCog(TTTcog *cog);
    void DeleteCog(int ID);
    TTTcog *GetCog(int ID);

    virtual ~TTTengine();
};

#endif