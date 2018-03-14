//
// Created by James Noeckel on 3/6/18.
//

#include "ScreenspaceQuad.h"

static const float verts[] =
        {-1, 1,
         1, 1,
         -1, -1,
         1, -1};

static const float texCoords[] = {
        0, 1,
        1, 1,
        0, 0,
        1, 0
};

static const GLuint indices[] = {
        3, 1, 0,
        3, 0, 2
};

ScreenspaceQuad::ScreenspaceQuad() {

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &tbo);
    glBindBuffer(GL_ARRAY_BUFFER, tbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBindVertexArray(0);

    glBindVertexArray(0);
}

void ScreenspaceQuad::SetImage(GLuint tex) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);
}

void ScreenspaceQuad::Render() {
    program->Use();
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    program->Unuse();
}


ScreenspaceQuad::~ScreenspaceQuad() {
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &tbo);
    glDeleteBuffers(1, &ebo);
    glDeleteVertexArrays(1, &vao);
}

void ScreenspaceQuad::SetUniforms() {
    texUniform = program->GetUniformLocation("image");
    program->Use();
    glUniform1i(texUniform, 0);
    program->Unuse();
}

void ScreenspaceQuad::GetShader() {
    program = Program::GetSimpleShader();
}

void ScreenspaceQuad::Init() {
    GetShader();
    SetUniforms();
}
