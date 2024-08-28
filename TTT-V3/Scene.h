#ifndef SCENE_H
#define SCENE_H

#include "Model.h"
#include "Terminal.h"
#include <vector>
#include <mutex>

class Scene
{
private:
    std::vector<Model*> *m_models;

    std::string TerminalLoadModel(void *v);
    std::string TerminalUnloadModel(void *v);
    std::string TerminalRotateModel(void *v);
    std::string TerminalScaleModel(void *v);
    std::string TerminalTranslateModel(void *v);

    std::mutex m_modelMutex;
public:
    Scene();
    
    bool LoadModel(std::string fname);
    bool AddModel(Model *model);
    bool UnloadModel(unsigned int index);
    void DrawModels(Shader *shader, Camera *camera);
    Model *ModelAt(unsigned int index);

    virtual ~Scene();
};

#endif