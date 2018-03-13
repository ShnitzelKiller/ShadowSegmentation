//
// Created by James Noeckel on 3/6/18.
//

#ifndef RENDERER_SCREENSPACEQUAD_H
#define RENDERER_SCREENSPACEQUAD_H


#include <GL/glew.h>
#include "gl/shaders.h"

class ScreenspaceQuad {
public:
    ScreenspaceQuad();
    void SetImage(GLuint tex);
    void Render(bool invert = false);
    ~ScreenspaceQuad();
private:
    void SetupVertexData();
    Program *program;
    GLint texUniform;
    GLint invertUniform;
    GLuint vao;
    GLuint vbo;
    GLuint tbo;
    GLuint ebo;
};


#endif //RENDERER_SCREENSPACEQUAD_H
