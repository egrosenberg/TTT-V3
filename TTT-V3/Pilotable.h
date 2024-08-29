#ifndef PILOTABLE_H
#define PILOTABLE_H

#include "TTTobject.h"
#include "Camera.h"

class Pilotable : public TTTobject
{
private:
    Camera *m_camera;
    GLFWwindow *m_window;
public:
    Pilotable(Scene* scene, Camera *camera, GLFWwindow *window);

    void FixedUpdate();

    virtual ~Pilotable();
};

#endif