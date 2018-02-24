//
// Created by James Noeckel on 2/2/18.
//


#include <tiny_obj_loader.h>
#include "Scene.h"
#include <glm/gtc/matrix_transform.hpp>


glm::mat4 DirectionalLight::GetProjectionMatrix(int xmin, int ymin, int xmax, int ymax) {
    glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0));
    glm::mat4 proj = glm::ortho(xmin, xmax, ymin, ymax);
    float shearX = dir.x/dir.z;
    float shearY = dir.y/dir.z;
    glm::mat4 shear(1, 0, -shearX, 0, 0, 1, -shearY, 0, 0, 0, 1, 0, 0, 0, 0, 1);
    return proj * view * shear;
}


Scene::Scene(int xmin, int ymin, int xmax, int ymax) : xmin(xmin), ymin(ymin), xmax(xmax), ymax(ymax) {
    meshes = std::vector<Mesh>();
    instances = std::vector<Instance>();
    cameras = std::vector<Camera>();
    materials = std::vector<Material>();
}

void Scene::AddInstance(size_t meshID, size_t *newInstanceID) {
    Instance inst;
    inst.meshID = meshID;
    inst.Scale = glm::vec3(1,1,1);
    inst.RotationOrigin = glm::vec3(0,0,0);
    inst.Rotation = glm::angleAxis(0.f, glm::vec3(0.f, 1.f, 0.f));
    inst.Translation = glm::vec3(0,0,0);
    instances.push_back(inst);
    *newInstanceID = instances.size() - 1;
}

void Scene::AddInstance(size_t meshID, glm::vec3 scale, glm::vec3 rotationOrigin, glm::quat rotation, glm::vec3 translation, size_t *newInstanceID) {
    Instance inst;
    inst.meshID = meshID;
    inst.Scale = scale;
    inst.RotationOrigin = rotationOrigin;
    inst.Rotation = rotation;
    inst.Translation = translation;
    instances.push_back(inst);
    *newInstanceID = instances.size() - 1;
}

void Scene::AddDirectionalLight(glm::vec3 dir) {
    auto light = new DirectionalLight();
    light->dir = dir;
    lights.push_back(light);
}

void Scene::LoadMesh(const std::string &filename) {
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    tinyobj::attrib_t attrib;
    std::string err;
    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename.c_str())) {
        fprintf(stderr, "failed to load %s", filename.c_str());
        return;
    }
    GLuint positionVBO = 0;
    GLuint texcoordVBO = 0;
    GLuint normalVBO = 0;
    GLuint indicesEBO = 0;

    if (!attrib.vertices.empty()) {
        glGenBuffers(1, &positionVBO);
        glBindBuffer(GL_ARRAY_BUFFER, positionVBO);
        glBufferData(GL_ARRAY_BUFFER,
                     attrib.vertices.size() * sizeof(float),
                     attrib.vertices.data(),
                     GL_STATIC_DRAW
        );
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    if (!attrib.texcoords.empty())
    {
        glGenBuffers(1, &texcoordVBO);
        glBindBuffer(GL_ARRAY_BUFFER, texcoordVBO);
        glBufferData(GL_ARRAY_BUFFER,
                     attrib.texcoords.size() * sizeof(float),
                     attrib.texcoords.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    if (!attrib.normals.empty())
    {
        glGenBuffers(1, &normalVBO);
        glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
        glBufferData(GL_ARRAY_BUFFER,
                     attrib.normals.size() * sizeof(float),
                     attrib.normals.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    if (!shapes[0].mesh.indices.empty())
    {
        glGenBuffers(1, &indicesEBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     shapes[0].mesh.indices.size() * sizeof(unsigned int),
                     shapes[0].mesh.indices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    //build vertex array object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    if (positionVBO != 0) {
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, positionVBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);
    }
    if (texcoordVBO != 0) {
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, texcoordVBO);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glEnableVertexAttribArray(1);
        glBindVertexArray(0);
    }
    if (normalVBO != 0)
    {
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glEnableVertexAttribArray(2);
        glBindVertexArray(0);
    }
    if (indicesEBO != 0) {
        glBindVertexArray(vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesEBO);
        glBindVertexArray(0);
    }

    Mesh mesh;
    mesh.name = filename;
    mesh.MeshVAO = vao;
    mesh.PositionBO = positionVBO;
    mesh.TexCoordBO = texcoordVBO;
    mesh.NormalBO = normalVBO;
    mesh.IndexBO = indicesEBO;
    mesh.VertexCount = (GLuint) attrib.vertices.size() / 3;
    mesh.IndexCount = (GLuint) shapes[0].mesh.indices.size();
    meshes.push_back(mesh);
}