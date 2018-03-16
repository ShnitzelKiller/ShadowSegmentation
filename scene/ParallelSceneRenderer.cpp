//
// Created by James Noeckel on 3/6/18.
//

#include "ParallelSceneRenderer.h"

ParallelSceneRenderer::ParallelSceneRenderer(float xmin, float ymin, float xmax, float ymax, int width, int height, GLenum internalformat) {
    s1 = new Scene(xmin, ymin, xmax, ymax);
    s2 = new Scene(xmin, ymin, xmax, ymax);
    r1 = new Renderer(s1, width, height, internalformat);
    r2 = new Renderer(s2, width, height, internalformat);
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

size_t ParallelSceneRenderer::AddInstance(size_t meshesID) {
    size_t id1 = s1->AddInstance(meshesID);
    size_t id2 = s2->AddInstance(meshesID);
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

void ParallelSceneRenderer::Render1(bool shading) {
    r1->Render(shading);
}

void ParallelSceneRenderer::Render2(bool shading) {
    r2->Render(shading);
}

void ParallelSceneRenderer::Render(bool shading) {
    r1->Render(shading);
    r2->Render(shading);
}

void ParallelSceneRenderer::GetImages(std::vector<GLuint> &imgs1, std::vector<GLuint> &imgs2) {
    for (auto im : r1->GetImages()) {
        imgs1.push_back(im);
    }
    for (auto im : r2->GetImages()) {
        imgs2.push_back(im);
    }
}

void ParallelSceneRenderer::SetVisible1(size_t instance, bool vis) {
    s1->SetVisible(instance, vis);
}

void ParallelSceneRenderer::SetVisible2(size_t instance, bool vis) {
    s2->SetVisible(instance, vis);
}

int ParallelSceneRenderer::GetHeight() const {
    return r1->GetWidth();
}

int ParallelSceneRenderer::GetWidth() const {
    return r1->GetHeight();
}

void ParallelSceneRenderer::ReadImageData1(void *buffer, GLenum format, GLenum type, int index) {
    r1->ReadImageData(buffer, format, type, index);
}

void ParallelSceneRenderer::ReadImageData2(void *buffer, GLenum format, GLenum type, int index) {
    r2->ReadImageData(buffer, format, type, index);
}

void ParallelSceneRenderer::GetFinalImages(GLuint *img1, GLuint *img2) {
    *img1 = r1->GetFinalImageID();
    *img2 = r2->GetFinalImageID();
}
