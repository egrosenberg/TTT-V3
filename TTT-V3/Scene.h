#ifndef SCENE_H
#define SCENE_H

#include "Model.h"
#include "Terminal.h"
#include <vector>

class Scene
{
private:
    std::vector<Model*> *m_models;

    std::string TerminalLoadModel(void *v);
    std::string TerminalUnloadModel(void *v);
    std::string TerminalRotateModel(void *v);
    std::string TerminalScaleModel(void *v);
    std::string TerminalTranslateModel(void *v);
public:
    Scene();
    
    bool LoadModel(std::string fname);
    bool UnloadModel(unsigned int index);
    void DrawModels(Shader *shader, Camera *camera);

    virtual ~Scene();
};

#endif