//
// Created by James Noeckel on 3/6/18.
//

#include <cstdio>
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

void ScreenspaceQuad::Render() {
    program->Use();
    SetUniforms();
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

void ScreenspaceQuad::Init() {
    GetShader();
    GetUniforms();
}

ScreenspaceQuad::ScreenspaceQuad(ScreenspaceQuad &&other) noexcept : vbo(other.vbo), tbo(other.tbo), ebo(other.ebo), vao(other.vao) {
    other.vbo = 0;
    other.tbo = 0;
    other.ebo = 0;
    other.vao = 0;
}
