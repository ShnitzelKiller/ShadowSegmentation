//
// Created by James Noeckel on 3/6/18.
//

#include "ParallelSceneRenderer.h"

ParallelSceneRenderer::ParallelSceneRenderer(float xmin, float ymin, float xmax, float ymax, int width, int height) {
    s1 = new Scene(xmin, ymin, xmax, ymax);
    s2 = new Scene(xmin, ymin, xmax, ymax);
    r1 = new Renderer(s1, width, height);
    r2 = new Renderer(s2, width, height);
}

ParallelSceneRenderer::~ParallelSceneRenderer() {
    delete r1;
    delete r2;
    delete s1;
    delete s2;
}

size_t ParallelSceneRenderer::LoadMesh(const std::string name1, const std::string name2) {
    size_t id1 = s1->LoadMesh(name1);
    size_t id2 = s2->LoadMesh(name2);
    assert(id1 == id2);
    return id1;
}

size_t ParallelSceneRenderer::AddInstance(size_t meshesID, glm::vec3 scale, glm::vec3 rotationOrigin,
                                          glm::quat rotation, glm::vec3 translation) {
    size_t id1 = s1->AddInstance(meshesID, scale, rotationOrigin, rotation, translation);
    size_t id2 = s2->AddInstance(meshesID, scale, rotationOrigin, rotation, translation);
    assert(id1 == id2);
    return id1;
}

void ParallelSceneRenderer::AddLight(Light *light) {
    s1->AddLight(light);
    s2->AddLight(light);
}

void
ParallelSceneRenderer::SetTransform(size_t instanceID, glm::vec3 scale, glm::vec3 rotationOrigin, glm::quat rotation,
                                    glm::vec3 translation) {
    s1->SetTransform(instanceID, scale, rotationOrigin, rotation, translation);
    s2->SetTransform(instanceID, scale, rotationOrigin, rotation, translation);
}
