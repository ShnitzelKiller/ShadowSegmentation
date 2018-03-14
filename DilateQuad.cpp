//
// Created by James Noeckel on 3/13/18.
//

#include <cstdio>
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
    glUniform1i(widthUniform, width);
    glUniform1i(heightUniform, height);
    glUniform1i(radUniform, radius);
    program->Unuse();
}

void DilateQuad::Render() {
    program->Use();
    glUniform1i(dimUniform, 0);

    GLint drawFBO;
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &drawFBO);
    rt.Bind();
    GLint m_viewport[4];
    glGetIntegerv( GL_VIEWPORT, m_viewport );
    glViewport(0, 0, width, height);
    ScreenspaceQuad::Render();
    glViewport(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);

    //draw final pass to the original framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, drawFBO);
    program->Use();
    glUniform1i(dimUniform, 1);

    GLuint origTex = textureID;
    SetImage(rt.GetTextureIDs()[0]);
    ScreenspaceQuad::Render();
    SetImage(origTex);
}

DilateQuad::DilateQuad(int width, int height, int radius) : width(width), height(height), radius(radius), rt(width, height, 1, GL_RGBA){

}

void DilateQuad::SetRadius(int rad) {
    program->Use();
    glUniform1i(radUniform, rad);
    program->Unuse();
}
