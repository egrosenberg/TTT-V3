#ifndef MODEL_H
#define MODEL_H

#include <json/json.h>
#include <math.h>
#include "Mesh.h"


using json = nlohmann::json;

// gltf model loader
class Model
{
private:
    std::string m_fname;
    std::vector<unsigned char> *m_data;
    json m_JSON;
    glm::mat4 m_transform;
    glm::quat m_rotation_quat;
    glm::vec3 m_rotation_euler;
    glm::vec3 m_scale;
    glm::vec3 m_translation;

    void UpdateMatrix();

    std::vector<Mesh*> *m_meshes;
    std::vector<glm::vec3>	*m_translationsMeshes;
    std::vector<glm::quat>	*m_rotationsMeshes;
    std::vector<glm::vec3>	*m_scalesMeshes;
    std::vector<glm::mat4>	*m_matricesMeshes;

    std::vector<std::string> *m_loadedTexName;
    std::vector<Texture*> *m_loadedTex;

    void LoadMesh(unsigned int indMesh);

    void TraverseNode(unsigned int nextNode, glm::mat4 matrix = glm::mat4(1.0f));

    std::vector<unsigned char> *GetData();
    std::vector<float> GetFloats(json accessor);
    std::vector<GLuint> GetIndices(json accessor);
    std::vector<Texture*> GetTextures();

    std::vector<Vertex> AssembleVertices
    (
        std::vector<glm::vec3> positions,
        std::vector<glm::vec3> normals,
        std::vector<glm::vec2> texUVs
    );

    std::vector<glm::vec2> GroupFloatsVec2(std::vector<float> floatVec);
    std::vector<glm::vec3> GroupFloatsVec3(std::vector<float> floatVec);
    std::vector<glm::vec4> GroupFloatsVec4(std::vector<float> floatVec);
public:
    Model(const char* fname);

    void Draw(Shader* shader, Camera* camera);

    glm::quat GetRotationQuat() { return m_rotation_quat; }
    glm::vec3 GetRotationEuler() { return m_rotation_euler; }
    glm::vec3 GetScale() { return m_scale; }
    glm::vec3 GetPosition() { return m_translation; }

    void Rotate(glm::vec3 vec);
    void Scale(glm::vec3 vec);
    void Translate(glm::vec3 vec);

    void SetEuler(glm::vec3 eulerValues);
    void SetScale(glm::vec3 scale);
    void SetPosition(glm::vec3 position);

    std::vector<Mesh*> *Meshes() { return m_meshes; };

    virtual ~Model();
};

#endif