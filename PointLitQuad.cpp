//
// Created by James Noeckel on 3/15/18.
//

#include <cstdio>
#include "PointLitQuad.h"

void PointLitQuad::SetBounds(float xmin, float ymin, float xmax, float ymax) {
    bounds = glm::vec4(xmin, ymin, xmax, ymax);
}

void PointLitQuad::SetLightPos(float x, float y, float z) {
    lightPos = glm::vec3(x, y, z);
}

void PointLitQuad::SetIntensity(float quadratic, float linear, float ambient) {
    lightIntensity = glm::vec3(ambient, linear, quadratic);
}

void PointLitQuad::GetUniforms() {
    lightPosUniform = program->GetUniformLocation("pos");
    boundsUniform = program->GetUniformLocation("bounds");
    intensityUniform = program->GetUniformLocation("intensity");
}

void PointLitQuad::GetShader() {
    program = Program::GetPointLitShader();
}

void PointLitQuad::SetUniforms() {
    glUniform4fv(boundsUniform, 1, &bounds[0]);
    glUniform3fv(lightPosUniform, 1, &lightPos[0]);
    glUniform3fv(intensityUniform, 1, &lightIntensity[0]);
}
