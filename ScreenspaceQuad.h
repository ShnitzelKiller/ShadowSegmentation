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
    void Render();
    ~ScreenspaceQuad();
private:
    Program program;
    GLint texUniform;
    GLuint vao;
    GLuint vbo;
    GLuint tbo;
    GLuint ebo;
};


#endif //RENDERER_SCREENSPACEQUAD_H
