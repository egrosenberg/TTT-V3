#include "TTTobject.h"

TTTobject::TTTobject(Scene *scene)
{
    o_model = nullptr;
    o_scene = scene;

    o_position = glm::vec3(0.0f);   // meters
    o_rotation = glm::vec3(0.0f);   // radians
    o_scale    = glm::vec3(1.0f);   // percentage
}

/**
 * Load model from specified file
 * 
 * @param fname: path to model file
 * @return true if successful, false if not
 */
bool TTTobject::LoadModel(const char *fname)
{
    // check if we need to add model to scene
    bool addToScene = !o_model;
    try
    {
        o_model = new Model(fname);
    }
    catch (...)
    {
        o_model = nullptr;
        return false;
    }
    o_scene->AddModel(o_model);
    return true;
}

/**
 * Update model position
 */
void TTTobject::FixedUpdate()
{
    TTTphysics::FixedUpdate();
    if (o_model)
    {
        o_model->SetPosition(p_position + o_position);
        o_model->SetEuler(p_rotation + o_rotation);
        o_model->SetScale(o_scale);
    }
}

TTTobject::~TTTobject()
{
    if (o_model)
    {
        delete o_model;
    }
}