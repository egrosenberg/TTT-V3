#include "TTTengine.h"

// ensure singleton instance is null
TTTengine *TTTengine::m_singleton = nullptr;

/**
 * Get and return singleton instance of engine
 * create singleton instance if it doesn't exist yet
 * 
 * @return pointer to singleton instance
 */
TTTengine *TTTengine::GetSingleton()
{
    if (!m_singleton)
    {
        m_singleton = new TTTengine();
    }
    return m_singleton;
}

// Constructor
TTTengine::TTTengine()
{
    m_CID = 0;
    m_cogs = new std::vector<TTTcog*>();
    m_cogQueue = new std::vector<TTTcog*>();
    m_cogsMutex = new std::mutex();
    m_cogQueueMutex = new std::mutex();
    m_startTime = glfwGetTime();
    m_isRunning = true;

    m_fixedUpdateThread = new std::thread(FixedUpdateThreadProc, this);
    m_graphicUpdateThread = nullptr;
}

/**
 * Fixed update thread process
 * Run fixed updates of all cogs at set interval
 * 
 * @param sharedData: pointer to engine running the thread
 */
void TTTengine::FixedUpdateThreadProc(TTTengine *engine)
{
    // make sure param isnt null
    if (!engine)
    {
        std::cerr << "Failed to start FixedUpdate thread. Nullptr arg." << std::endl;
        return;
    }

    // get time for first tick
    double nextTick = engine->m_startTime + TICK_TIME;

    // run updates continuously
    while (engine->m_isRunning)
    {
        // only run fixed updates after a tick is available
        if (glfwGetTime() >= nextTick)
        {
            nextTick += TICK_TIME;
            // lock cogs mutex
            engine->m_cogsMutex->lock();
            // iterate through cogs
            for (std::vector<TTTcog*>::iterator cog = engine->m_cogs->begin();
                    cog != engine->m_cogs->end(); ++cog)
            {
                (*cog)->FixedUpdate(); // call cogs fixed update
            }
            // unlock cogs mutex
            engine->m_cogsMutex->unlock();
            // lock cog queue mutex
            engine->m_cogQueueMutex->lock();
            // update cogs list from queue
            while (engine->m_cogQueue->size() > 0)
            {
                TTTcog* temp = engine->m_cogQueue->back();
                engine->m_cogQueue->pop_back();
                engine->m_cogs->push_back(temp);
            }
            //unlock cog queue mutex
            engine->m_cogQueueMutex->unlock();
        }
    }
}
/**
 * Graphical Update thread process
 * Run updates of all cogs at set interval
 * Handle the drawing process
 *
 * @param sharedData: pointer to engine running the thread
 */
void TTTengine::GraphicalUpdateThreadProc(TTTengine *engine)
{
    // make sure param isnt null
    if (!engine)
    {
        std::cerr << "Failed to start GraphicUpdate thread. Nullptr arg." << std::endl;
        return;
    }

    // TODO: do the drawing process as well as running non-fixed updates


}

/**
 * Creates a new cog and adds it engine
 * 
 * @param cog: pointer to cog to add to engine
 * @param type: type_info for the cog's class
 * @return ID of cog
 */
int TTTengine::BuildCog(TTTcog *cog)
{
    m_cogQueueMutex->lock();
    m_cogQueue->push_back(cog);
    int id = m_CID;
    ++m_CID;
    m_cogQueueMutex->unlock();
    return id;
}

/**
 * Deletes the cog of the specified ID if it exists
 * 
 * @param ID: int ID to delete
 */
void TTTengine::DeleteCog(int ID)
{
    m_cogsMutex->lock();
    // Check to see if ID is a valid index
    if (m_cogs->size() < ID && (*m_cogs)[ID])
    {
        // delete cog and set pointer to nullptr
        delete (*m_cogs)[ID];
        (*m_cogs)[ID] = nullptr;
    }
    m_cogsMutex->unlock();
}

/**
 * Retrieve cog of specified id
 * 
 * @param ID: id of the cog the program wants
 * @return pointer to requested cog or nullptr if it doesnt exist
 */
TTTcog *TTTengine::GetCog(int ID)
{
    m_cogsMutex->lock();
    // check validity of ID
    if (ID >= m_cogs->size())
    {
        m_cogsMutex->unlock();
        return nullptr;
    }
    TTTcog *ret = (*m_cogs)[ID];
    m_cogsMutex->unlock();
    return ret;
}

// Destructor
TTTengine::~TTTengine()
{
    m_isRunning = false;
    m_fixedUpdateThread->join();

    delete m_cogs;
    delete m_fixedUpdateThread;
    delete m_graphicUpdateThread;
    delete m_cogsMutex;
    delete m_cogQueueMutex;
}