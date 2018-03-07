//
// Created by James Noeckel on 2/2/18.
//


#include <tiny_obj_loader.h>
#include "Scene.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "../mathdebug.h"


glm::mat4 DirectionalLight::GetProjectionMatrix(float xmin, float ymin, float xmax, float ymax) {
    //glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0));
    glm::mat4 proj = glm::ortho(xmin, xmax, ymin, ymax, 0.f, -10.f);
    proj[2][2] = 0;
    proj[3][2] = 0;
    float shearX = dir.x/dir.z;
    float shearY = dir.y/dir.z;
    glm::mat4 shear;
    shear[2][0] = -shearX;
    shear[2][1] = -shearY;
    return proj * shear;
}

glm::mat4 PointLight::GetProjectionMatrix(float xmin, float ymin, float xmax, float ymax) {
    glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0));
    glm::mat4 proj = glm::ortho(xmin, xmax, ymin, ymax, 0.f, -10.f);
    proj[2][2] = 0;
    proj[3][2] = 0;
    proj[2][3] = -1.f/pos.z;
    float shearX = pos.x/pos.z;
    float shearY = pos.y/pos.z;
    glm::mat4 shear;
    shear[2][0] = -shearX;
    shear[2][1] = -shearY;
    return proj * shear;
}


Scene::Scene(float xmin, float ymin, float xmax, float ymax) : xmin(xmin), ymin(ymin), xmax(xmax), ymax(ymax) { }

size_t Scene::AddInstance(size_t meshID) {
    return AddInstance(meshID, glm::vec3(1,1,1), glm::vec3(0,0,0), glm::angleAxis(0.f, glm::vec3(0.f, 1.f, 0.f)), glm::vec3(0,0,0));
}

size_t Scene::AddInstance(size_t meshID, glm::vec3 scale, glm::vec3 rotationOrigin, glm::quat rotation, glm::vec3 translation) {
    Instance inst;
    inst.meshID = meshID;
    inst.Scale = scale;
    inst.RotationOrigin = rotationOrigin;
    inst.Rotation = rotation;
    inst.Translation = translation;
    instances.push_back(inst);
    return instances.size() - 1;
}

void Scene::SetTransform(size_t instanceID, glm::vec3 scale, glm::vec3 rotationOrigin, glm::quat rotation,
                           glm::vec3 translation) {
    Instance &inst = instances[instanceID];
    inst.Scale = scale;
    inst.RotationOrigin = rotationOrigin;
    inst.Rotation = rotation;
    inst.Translation = translation;
    instances.push_back(inst);
}

/*void Scene::AddDirectionalLight(glm::vec3 dir) {
    auto light = new DirectionalLight();
    light->dir = dir;
    lights.push_back(light);
}*/

void Scene::AddLight(Light *light) {
    lights.push_back(light);
}

size_t Scene::AddTri(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3) {
    float pos[] = {x1, y1, z1,
                   x2, y2, z2,
                   x3, y3, z3};
    float tex[] = {0, 0,
                   1, 0,
                   0, 1};

    glm::vec3 d1(x2-x1, y2-y1, z2-z1);
    glm::vec3 d2(x3-x1, y3-y1, z3-z1);
    glm::vec3 n = glm::cross(d1, d2);

    GLuint ind[] = {
            0, 1, 2,
            2, 1, 0
    };
    GLuint positionVBO = 0;
    GLuint indicesEBO = 0;
    GLuint texcoordVBO = 0;
    GLuint normalVBO = 0;
    GLuint vao = 0;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &positionVBO);
    glBindBuffer(GL_ARRAY_BUFFER, positionVBO);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), pos, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &texcoordVBO);
    glBindBuffer(GL_ARRAY_BUFFER, texcoordVBO);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), tex, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &normalVBO);
    glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
    glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float), &n[0], GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);

    glGenBuffers(1, &indicesEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), ind, GL_STATIC_DRAW);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    Mesh mesh;
    mesh.name = "tri";
    mesh.MeshVAO = vao;
    mesh.PositionBO = positionVBO;
    mesh.TexCoordBO = texcoordVBO;
    mesh.NormalBO = normalVBO;
    mesh.IndexBO = indicesEBO;
    mesh.VertexCount = (GLuint) 3;
    mesh.IndexCount = (GLuint) 6;
    meshes.push_back(mesh);
    return meshes.size() - 1;
}


size_t Scene::LoadMesh(const std::string &filename) {
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    tinyobj::attrib_t attrib;
    std::string err;
    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename.c_str())) {
        fprintf(stderr, "failed to load %s: %s", filename.c_str(), err.c_str());
        return 0;
    }
    GLuint positionVBO = 0;
    GLuint texcoordVBO = 0;
    GLuint normalVBO = 0;
    GLuint indicesEBO = 0;

    if (!attrib.vertices.empty()) {
        std::cout << "loaded " << attrib.vertices.size() << " vertices" << std::endl;
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
        std::cout << "loaded " << attrib.texcoords.size() << " texture coordinates" << std::endl;
        glGenBuffers(1, &texcoordVBO);
        glBindBuffer(GL_ARRAY_BUFFER, texcoordVBO);
        glBufferData(GL_ARRAY_BUFFER,
                     attrib.texcoords.size() * sizeof(float),
                     attrib.texcoords.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    if (!attrib.normals.empty())
    {
        std::cout << "loaded " << attrib.normals.size() << " normals" << std::endl;
        glGenBuffers(1, &normalVBO);
        glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
        glBufferData(GL_ARRAY_BUFFER,
                     attrib.normals.size() * sizeof(float),
                     attrib.normals.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    if (!shapes[0].mesh.indices.empty())
    {
        std::cout << "loaded " << shapes[0].mesh.indices.size() << " mesh indices" << std::endl;
        GLuint vind[shapes[0].mesh.indices.size()];
        for (int i=0; i<shapes[0].mesh.indices.size(); i++) {
            auto ind = shapes[0].mesh.indices[i];
            vind[i] = (GLuint) ind.vertex_index;
        }
        glGenBuffers(1, &indicesEBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     sizeof(vind),
                     vind, GL_STATIC_DRAW);
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
    return meshes.size() - 1;
}

Scene::~Scene() {
    /*for (auto it=lights.begin(); it != lights.end(); it++) {
        delete *it;
    }*/
    for (Mesh m : meshes) {
        glDeleteVertexArrays(1, &m.MeshVAO);
        glDeleteBuffers(1, &m.IndexBO);
        glDeleteBuffers(1, &m.NormalBO);
        glDeleteBuffers(1, &m.TexCoordBO);
        glDeleteBuffers(1, &m.PositionBO);
    }
}

glm::mat4 Scene::GetProjectionMatrix(size_t i) {
    return lights[i]->GetProjectionMatrix(xmin, ymin, xmax, ymax);
}

void Scene::SetVisible(size_t instanceID, bool visible) {
    instances[instanceID].active = visible;
}
