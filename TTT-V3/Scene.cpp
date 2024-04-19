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
 * Terminal function for rotate a model
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
    // extract rotation values (as floats)
    float x = vec->y;
    float y = vec->z;
    float z = vec->w;
    // create a matrix for the transformation
    glm::mat4 transformation = glm::mat4(1.0f);
    glm::vec3 rotation_x = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 rotation_y = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 rotation_z = glm::vec3(0.0f, 0.0f, 1.0f);
    // rotate on each axis according to rotaiton value
    transformation = glm::rotate(transformation, glm::radians(x), rotation_x);
    transformation = glm::rotate(transformation, glm::radians(y), rotation_y);
    transformation = glm::rotate(transformation, glm::radians(z), rotation_z);
    m_models->at(index)->SetTransform(transformation);
    return "rotated model";
}
/**
 * Terminal function for rotate a model
 *
 * @param v: vec4 cast to void pointer formatted as [index, x, y, z]
 * @return: success message or error message
 */
std::string Scene::TerminalScaleModel(void* v)
{
    // check to make sure it isn't nullptr
    if (!v)
    {
        return "invalid pointer.";
    }
    return "todo: impliment this function";
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
    return "todo: impliment this function";
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
