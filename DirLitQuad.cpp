//
// Created by James Noeckel on 3/15/18.
//

#include <cstdio>
#include "DirLitQuad.h"

void DirLitQuad::SetDir(float dx, float dy, float dz) {
    dir = glm::vec3(dx, dy, dz);
}

void DirLitQuad::SetIntensity(float i) {
    intensity = i;
}

void DirLitQuad::GetUniforms() {
    dirUniform = program->GetUniformLocation("dir");
    intensityUniform = program->GetUniformLocation("intensity");
}

void DirLitQuad::SetUniforms() {
    glUniform1f(intensityUniform, intensity);
    glUniform3fv(dirUniform, 1, &dir[0]);
}

void DirLitQuad::GetShader() {
    program = Program::GetDirLitShader();
}


