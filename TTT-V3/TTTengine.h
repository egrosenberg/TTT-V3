#ifndef TTT_ENGINE_H
#define TTT_ENGINE_H

#define TICK_TIME 0.02f

#include <vector>
#include <utility>
#include <typeinfo>
#include <iostream>
#include "TTT.h"
#include "TTTcog.h"
#include "Terminal.h"
#include <thread>
#include <mutex>

class TTTcog;

class TTTengine
{
private:

    TTTengine();
    static TTTengine *m_singleton;

    unsigned int m_CID;
    std::vector<TTTcog*> *m_cogs;
    std::vector<TTTcog*> *m_cogQueue;

    double m_startTime;
    bool m_isRunning;

    std::thread *m_fixedUpdateThread;
    std::thread *m_graphicUpdateThread;
    std::mutex *m_cogsMutex;
    std::mutex *m_cogQueueMutex;

    static void FixedUpdateThreadProc(TTTengine *engine);
    static void GraphicalUpdateThreadProc(TTTengine *engine);

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