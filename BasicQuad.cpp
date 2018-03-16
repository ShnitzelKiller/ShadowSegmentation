//
// Created by James Noeckel on 3/13/18.
//

#include <iostream>
#include "BasicQuad.h"



void BasicQuad::GetUniforms() {
    //ScreenspaceQuad::GetUniforms();
    texUniform = program->GetUniformLocation("image");
    invertUniform = program->GetUniformLocation("invert");
}

void BasicQuad::SetImage(GLuint tex) {
    textureID = tex;
}

void BasicQuad::GetShader() {
    program = Program::GetSimpleShader();
}

void BasicQuad::SetInvert(bool inv) {
    invert = inv;
}

void BasicQuad::SetUniforms() {
    glUniform1f(invertUniform, invert ? 1.0f : 0.0f);
    glUniform1i(texUniform, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
}
