#include "TTTcog.h"
#include <typeinfo>

/**
 * Instantiates cog in engine
 * ID will have a negative value if there is an issue
 */
TTTcog::TTTcog()
{
    // get ID / build cog into engine
    TTTengine *engine = TTTengine::GetSingleton();
    if (engine)
    {
        c_ID = engine->BuildCog(this);
    }
    else
    {
        c_ID = -1;
    }
}

/**
 * Runs function from this cog's map
 * 
 * @param id: id of function to return
 * @return: true only if the function exists runs
 */
//bool TTTcog::InvokeFn(unsigned int id)
//{
//    // check if function exists
//    std::map<unsigned int, std::function<TTT_GENERIC_FUNCTION>>::iterator fn = c_fMap->find(id);
//    if (fn == c_fMap->end())
//    {
//        return false;
//    }
//    (fn->second)();
//    return true;
//}

TTTcog::~TTTcog()
{
    // get ID / delete cog from engine
    TTTengine* engine = TTTengine::GetSingleton();
    engine->DeleteCog(c_ID);
}