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
    m_cogs->push_back(cog);
    int id = m_CID;
    ++m_CID;
    return id;
}

/**
 * Deletes the cog of the specified ID if it exists
 * 
 * @param ID: int ID to delete
 */
void TTTengine::DeleteCog(int ID)
{
    // Check to see if ID is a valid index
    if (m_cogs->size() < ID && (*m_cogs)[ID])
    {
        // delete cog and set pointer to nullptr
        delete (*m_cogs)[ID];
        (*m_cogs)[ID] = nullptr;
    }
}

/**
 * Retrieve cog of specified id
 * 
 * @param ID: id of the cog the program wants
 * @return pointer to requested cog or nullptr if it doesnt exist
 */
TTTcog *TTTengine::GetCog(int ID)
{
    // check validity of ID
    if (ID >= m_cogs->size())
    {
        return nullptr;
    }
    return (*m_cogs)[ID];
}

// Destructor
TTTengine::~TTTengine()
{
    delete m_cogs;
}