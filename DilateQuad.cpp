//
// Created by James Noeckel on 3/13/18.
//

#include "DilateQuad.h"



void DilateQuad::GetShader() {
    program = Program::GetDilateShader();
}

void DilateQuad::SetUniforms() {
    ScreenspaceQuad::SetUniforms();
    radUniform = program->GetUniformLocation("radius");
    dimUniform = program->GetUniformLocation("dim");
    widthUniform = program->GetUniformLocation("width");
    heightUniform = program->GetUniformLocation("height");
    program->Use();
    glUniform1i(widthUniform, 10);
    glUniform1i(heightUniform, 10);
    glUniform1i(radUniform, 1);
    program->Unuse();
}

void DilateQuad::Render() {
    program->Use();
    glUniform1i(dimUniform, 0);
    ScreenspaceQuad::Render();
    program->Use();
    glUniform1i(dimUniform, 1);
    ScreenspaceQuad::Render();
}

void DilateQuad::SetRadius(int rad) {
    program->Use();
    glUniform1i(radUniform, rad);
    program->Unuse();
}
