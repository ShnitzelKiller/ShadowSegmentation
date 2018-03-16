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
    virtual void Render();
    virtual ~ScreenspaceQuad();
    void Init();
private:
    GLuint vao;
    GLuint vbo;
    GLuint tbo;
    GLuint ebo;
protected:
    Program* program;
    GLint texUniform;
    virtual void GetUniforms() = 0;
    virtual void SetUniforms() = 0;
    virtual void GetShader() = 0;
};


#endif //RENDERER_SCREENSPACEQUAD_H
