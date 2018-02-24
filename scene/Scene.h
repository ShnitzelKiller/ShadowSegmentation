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
    virtual glm::mat4 GetProjectionMatrix(int xmin, int ymin, int xmax, int ymax) = 0;
    virtual ~Light() = default;
};

struct DirectionalLight : Light {
    glm::mat4 GetProjectionMatrix(int xmin, int ymin, int xmax, int ymax) override;
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
    Scene(int xmin, int ymin, int xmax, int ymax);
    std::vector<Mesh> meshes;
    std::vector<Instance> instances;
    std::vector<Camera> cameras;
    std::vector<Light*> lights;
    std::vector<Material> materials;
    int xmin, ymin, xmax, ymax;
    /**
     * Add an instance of the specified mesh, returning the instance ID in newInstanceID
     */
    void AddInstance(size_t meshID, size_t *newInstanceID);
    void AddInstance(size_t meshID, glm::vec3 scale, glm::vec3 rotationOrigin, glm::quat rotation, glm::vec3 translation, size_t *newInstanceID);

    void AddDirectionalLight(glm::vec3 dir);

    void LoadMesh(const std::string &filename);

    ~Scene() {
        for (auto it=lights.begin(); it != lights.end(); it++) {
            delete *it;
        }
        for (Mesh m : meshes) {
            glDeleteVertexArrays(1, &m.MeshVAO);
            glDeleteBuffers(1, &m.IndexBO);
            glDeleteBuffers(1, &m.NormalBO);
            glDeleteBuffers(1, &m.TexCoordBO);
            glDeleteBuffers(1, &m.PositionBO);
        }
    }
};

#endif //RENDERER_SCENE_H
