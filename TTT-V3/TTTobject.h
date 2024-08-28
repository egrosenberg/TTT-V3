#ifndef TTT_OBJECT_H
#define TTT_OBJECT_H

#include "TTTphysics.h"
#include "Model.h"
#include "Scene.h"

class TTTobject : public TTTphysics
{
private:
    Model *o_model;
    Scene *o_scene;

public:
    TTTobject(Scene *scene);

    bool LoadModel(const char *fname);

    void SetModelScale(glm::vec3 scale) { o_model->SetScale(scale); }

    void FixedUpdate();

    virtual ~TTTobject();
};

#endif