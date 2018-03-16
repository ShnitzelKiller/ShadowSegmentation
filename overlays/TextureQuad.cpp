//
// Created by James Noeckel on 3/13/18.
//

#include <iostream>
#include "TextureQuad.h"

void TextureQuad::GetUniforms() {
    //ScreenspaceQuad::GetUniforms();
    texUniform = program->GetUniformLocation("image");
    invertUniform = program->GetUniformLocation("invert");
}

void TextureQuad::SetImage(GLuint tex) {
    textureID = tex;
}

void TextureQuad::GetShader() {
    program = Program::GetSimpleShader();
}

void TextureQuad::SetInvert(bool inv) {
    invert = inv;
}

void TextureQuad::SetUniforms() {
    glUniform1f(invertUniform, invert ? 1.0f : 0.0f);
    glUniform1i(texUniform, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    invert = false;
}
