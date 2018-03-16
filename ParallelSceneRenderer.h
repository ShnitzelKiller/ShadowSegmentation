//
// Created by James Noeckel on 3/6/18.
//

#ifndef RENDERER_PARALLELSCENERENDERER_H
#define RENDERER_PARALLELSCENERENDERER_H


#include <string>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include "scene/Renderer.h"

class ParallelSceneRenderer {
public:
    ParallelSceneRenderer(float xmin, float ymin, float xmax, float ymax, int width, int height, GLenum internalformat = GL_RGBA);
    size_t LoadMesh(std::string name1, std::string name2);
    size_t AddInstance(size_t meshesID, glm::vec3 scale, glm::vec3 rotationOrigin, glm::quat rotation, glm::vec3 translation);
    void SetTransform(size_t instanceID, glm::vec3 scale, glm::vec3 rotationOrigin, glm::quat rotation, glm::vec3 translation);
    void AddLight(Light *light);
    void SetVisible1(size_t instance, bool vis);
    void SetVisible2(size_t instance, bool vis);
    void Render(bool shading = true);
    void Render1(bool shading = true);
    void Render2(bool shading = true);
    int GetWidth() const;
    int GetHeight() const;
    void GetImages(std::vector<GLuint> &imgs1, std::vector<GLuint> &imgs2);
    void ReadImageData1(void *buffer, GLenum format, GLenum type, int index);
    void ReadImageData2(void *buffer, GLenum format, GLenum type, int index);
    ~ParallelSceneRenderer();
private:
    Renderer* r1;
    Renderer* r2;
    Scene* s1;
    Scene* s2;
};


#endif //RENDERER_PARALLELSCENERENDERER_H
