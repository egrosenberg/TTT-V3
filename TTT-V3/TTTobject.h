#ifndef TTT_OBJECT_H
#define TTT_OBJECT_H

#include "TTTphysics.h"
#include "Model.h"
#include "Scene.h"

class TTTobject : public TTTphysics
{
protected:
    Model *o_model;
    Scene *o_scene;

    glm::vec3 o_position;            // meters
    glm::vec3 o_rotation;            // radians
    glm::vec3 o_scale;               // percentage

public:
    TTTobject(Scene *scene);

    bool LoadModel(const char *fname);

    void SetObjectScale(glm::vec3 scale) { o_scale = scale; }
    void SetObjectPosition(glm::vec3 pos) { o_position = pos; }
    void SetObjectRotation(glm::vec3 rot) { o_rotation = rot; }

    void FixedUpdate();

    virtual ~TTTobject();
};

#endif