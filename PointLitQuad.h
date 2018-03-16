//
// Created by James Noeckel on 3/15/18.
//

#ifndef RENDERER_POINTLITQUAD_H
#define RENDERER_POINTLITQUAD_H


#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include "ScreenspaceQuad.h"

class PointLitQuad : public ScreenspaceQuad {
public:
    void SetBounds(float xmin, float ymin, float xmax, float ymax);
    void SetLightPos(float x, float y, float z);
    void SetIntensity(float quadratic, float linear = 0, float ambient = 0);
private:
protected:
    void GetUniforms() override;
    void SetUniforms() override;
    void GetShader() override;
    GLint lightPosUniform;
    GLint boundsUniform;
    GLint intensityUniform;
    glm::vec4 bounds;
    glm::vec3 lightPos;
    glm::vec3 lightIntensity;
};


#endif //RENDERER_POINTLITQUAD_H
