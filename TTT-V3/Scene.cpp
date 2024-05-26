#include "Scene.h"

Scene::Scene()
{
    m_models = new std::vector<Model*>();

    // bind terminal functions
    Terminal *terminal = Terminal::GetSingleton();
    std::function<TTT_GENERIC_FUNCTION> loadModelFn = std::bind(&Scene::TerminalLoadModel, this, std::placeholders::_1);
    terminal->BindFn("load", loadModelFn, TTTenum::TTT_STRING);
    std::function<TTT_GENERIC_FUNCTION> unloadModelFn = std::bind(&Scene::TerminalUnloadModel, this, std::placeholders::_1);
    terminal->BindFn("unload", unloadModelFn, TTTenum::TTT_UINT);
    std::function<TTT_GENERIC_FUNCTION> rotateModelFn = std::bind(&Scene::TerminalRotateModel, this, std::placeholders::_1);
    terminal->BindFn("rotate", rotateModelFn, TTTenum::TTT_VEC4F);
    std::function<TTT_GENERIC_FUNCTION> scaleModelFn = std::bind(&Scene::TerminalScaleModel, this, std::placeholders::_1);
    terminal->BindFn("scale", scaleModelFn, TTTenum::TTT_VEC4F);
    std::function<TTT_GENERIC_FUNCTION> translateModelFn = std::bind(&Scene::TerminalTranslateModel, this, std::placeholders::_1);
    terminal->BindFn("translate", translateModelFn, TTTenum::TTT_VEC4F);
}

/**
 * Function for terminal binding to load a model
 * 
 * @param v: string pointer cast to void containing model filename
 * @return terminal message for status of loaded model
 */
std::string Scene::TerminalLoadModel(void *v)
{
    // check to make sure it isn't nullptr
    if (!v)
    {
        return "invalid pointer.";
    }
    // cast pointer back to string
    std::string *fname = (std::string*)v;

    // attempt to load file and log status as a str
    bool status = LoadModel(*fname);
    std::string status_str = status ? " loaded" : " failed to load";

    // return final status message
    return *fname + status_str;
}
/**
 * Function for terminal binding to load a model
 *
 * @param v: unsigned int pointer cast to void containing index of model
 * @return terminal message for status of unloaded model
 */
std::string Scene::TerminalUnloadModel(void *v)
{
    // check to make sure it isn't nullptr
    if (!v)
    {
        return "invalid pointer.";
    }
    // cast pointer back to unsigned int
    unsigned int *index = (unsigned int*)v;
    // attempt to unload model
    if (UnloadModel(*index))
    {
        return "succesffully unloaded model";
    }
    return "error: invalid model index";
}

/**
 * Terminal function for rotating a model
 * 
 * @param v: vec4 cast to void pointer formatted as [index, x, y, z]
 * @return: success message or error message
 */
std::string Scene::TerminalRotateModel(void* v)
{
    // check to make sure it isn't nullptr
    if (!v)
    {
        return "invalid pointer.";
    }
    // cast back to vec4
    glm::vec4 *vec = (glm::vec4*)v;
    // extract index
    int index = (int)vec->x;
    // bounds checking
    if (index < 0 || index >= m_models->size())
    {
        return "error: index out of bounds";
    }
    // extract rotation values (as floats) converted to radians
    float x = glm::radians(vec->y);
    float y = glm::radians(vec->z);
    float z = glm::radians(vec->w);
    // create a vec3 of rotation values
    glm::vec3 rotation = glm::vec3(vec->y, vec->z, vec->w);
    // rotate model based on euler values
    m_models->at(index)->Rotate(glm::vec3(vec->y, vec->z, vec->w));
    return "rotated model";
}
/**
 * Terminal function for rotate a model
 *
 * @param v: vec4 cast to void pointer formatted as [index, x, y, z]
 * @return: success message or error message
 */
std::string Scene::TerminalScaleModel(void *v)
{
    // check to make sure it isn't nullptr
    if (!v)
    {
        return "invalid pointer.";
    }
    // cast back to vec4
    glm::vec4 *vec = (glm::vec4*)v;
    // extract index
    int index = (int)vec->x;
    // bounds checking
    if (index < 0 || index >= m_models->size())
    {
        return "error: index out of bounds";
    }
    // extract rotation values (as floats)
    float x = vec->y;
    float y = vec->z;
    float z = vec->w;
    // store scale values in vec3
    glm::vec3 scale = glm::vec3(x, y, z);
    // Scale model by vec values
    m_models->at(index)->Scale(scale);
    return "scaled model";
}
/**
 * Terminal function for rotate a model
 *
 * @param v: vec4 cast to void pointer formatted as [index, x, y, z]
 * @return: success message or error message
 */
std::string Scene::TerminalTranslateModel(void* v)
{
    // check to make sure it isn't nullptr
    if (!v)
    {
        return "invalid pointer.";
    }
    // cast back to vec4
    glm::vec4* vec = (glm::vec4*)v;
    // extract index
    int index = (int)vec->x;
    // bounds checking
    if (index < 0 || index >= m_models->size())
    {
        return "error: index out of bounds";
    }
    // extract rotation values (as floats)
    float x = vec->y;
    float y = vec->z;
    float z = vec->w;
    // create a vector of translation values
    glm::vec3 translation = glm::vec3(x, y, z);
    // translate model
    m_models->at(index)->Translate(translation);
    return "translated model";
}
/**
 * Loads a new model and adds to to models list
 * 
 * @param fname: path to glft file to load
 * @return true if successful, false otherwise
 */
bool Scene::LoadModel(std::string fname)
{
    try
    {
        m_models->push_back(new Model(fname.c_str()));
    }
    catch (...)
    {
        return false;
    }
}
/**
 * Unloads a model from the models list
 * 
 * @param index: index of model in vector
 * @return true on deletion, false on index out of bounds
 */
bool Scene::UnloadModel(unsigned int index)
{
    // check bounds
    if (index >= m_models->size())
    {
        return false;
    }
    m_models->erase(m_models->begin() + index);
}
/**
 * Draw each model stored in the model loader
 * 
 * @param shader: shader program to render with
 * @param camera: camera to render from
 */
void Scene::DrawModels(Shader* shader, Camera* camera)
{
    for (int i = 0; i < m_models->size(); ++i)
    {
        (*m_models)[i]->Draw(shader, camera);
    }
}

Scene::~Scene()
{
    delete m_models;
}
