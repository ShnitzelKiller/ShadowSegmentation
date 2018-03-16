//
// Created by James Noeckel on 3/15/18.
//

#ifndef RENDERER_DIRLITSHADER_H
#define RENDERER_DIRLITSHADER_H


#include <glm/vec3.hpp>
#include "ScreenspaceQuad.h"

class DirLitQuad : public ScreenspaceQuad {
public:
    void SetDir(float dx, float dy, float dz);
    void SetIntensity(float i);
private:
protected:
    void GetUniforms() override;
    void SetUniforms() override;
    void GetShader() override;
    GLint dirUniform;
    GLint intensityUniform;
    float intensity;
    glm::vec3 dir;
};


#endif //RENDERER_DIRLITSHADER_H
