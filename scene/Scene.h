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

struct Material
{
    std::string name;

    float Ambient[3];
    float Diffuse[3];
};

struct Camera
{
    // View
    glm::vec3 Eye;
    glm::vec3 Target;
    glm::vec3 Up;

    // Projection
    float FovY;
    float Aspect;
    float ZNear;
};

struct Light {
    virtual glm::mat4 GetProjectionMatrix(float xmin, float ymin, float xmax, float ymax) = 0;
    virtual ~Light() = default;
};

struct DirectionalLight : Light {
    glm::mat4 GetProjectionMatrix(float xmin, float ymin, float xmax, float ymax) override;
    glm::vec3 dir;
};

struct Instance {
    size_t meshID;

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
    std::vector<Camera> cameras;
    std::vector<Light*> lights;
    std::vector<Material> materials;
    float xmin, ymin, xmax, ymax;
    /**
     * Add an instance of the specified mesh, returning the instance ID in newInstanceID
     */
    size_t AddInstance(size_t meshID);
    size_t AddInstance(size_t meshID, glm::vec3 scale, glm::vec3 rotationOrigin, glm::quat rotation, glm::vec3 translation);

    void AddDirectionalLight(glm::vec3 dir);

    size_t LoadMesh(const std::string &filename);
    size_t AddTri(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);

    void Destroy();

    ~Scene() {
        for (auto it=lights.begin(); it != lights.end(); it++) {
            delete *it;
        }
    }
};

#endif //RENDERER_SCENE_H
