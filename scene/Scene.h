//
// Created by James Noeckel on 2/2/18.
//

#ifndef RENDERER_SCENE_H
#define RENDERER_SCENE_H

#include <string>
#include <vector>
#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

struct Mesh {
    std::string name;
    GLuint MeshVAO;
    GLuint PositionBO;
    GLuint TexCoordBO;
    GLuint NormalBO;
    GLuint IndexBO;

    GLuint IndexCount;
    GLuint VertexCount;

    std::vector<uint32_t> MaterialIDs;
};

struct Light {
    virtual glm::mat4 GetProjectionMatrix(float xmin, float ymin, float xmax, float ymax) = 0;
    virtual ~Light() = default;
};

struct DirectionalLight : Light {
    DirectionalLight(float dx, float dy, float dz) : dir(glm::vec3(dx, dy, dz)) {}
    glm::mat4 GetProjectionMatrix(float xmin, float ymin, float xmax, float ymax) override;
    glm::vec3 dir;
};

struct PointLight : Light {
    PointLight(float px, float py, float pz) : pos(glm::vec3(px, py, pz)) {}
    glm::mat4 GetProjectionMatrix(float xmin, float ymin, float xmax, float ymax) override;
    glm::vec3 pos;
};

struct Instance {
    size_t meshID;
    bool active = true;
    glm::vec3 Scale;
    glm::vec3 RotationOrigin;
    glm::quat Rotation;
    glm::vec3 Translation;
};

class Scene {
public:
    Scene(float xmin, float ymin, float xmax, float ymax);
    std::vector<Mesh> meshes;
    std::vector<Instance> instances;
    std::vector<Light*> lights;
    /**
     * Add an instance of the specified mesh, returning the instance ID in newInstanceID
     */
    size_t AddInstance(size_t meshID);
    size_t AddInstance(size_t meshID, glm::vec3 scale, glm::vec3 rotationOrigin, glm::quat rotation, glm::vec3 translation);
    void SetTransform(size_t instanceID, glm::vec3 scale, glm::vec3 rotationOrigin, glm::quat rotation, glm::vec3 translation);
    void SetVisible(size_t instanceID, bool visible);

    //void AddDirectionalLight(glm::vec3 dir);
    void AddLight(Light *light);
    glm::mat4 GetProjectionMatrix(size_t i);

    size_t LoadMesh(const std::string &filename);
    size_t AddTri(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);

    ~Scene();
private:
    float xmin, ymin, xmax, ymax;
    friend class Light;
};

#endif //RENDERER_SCENE_H
