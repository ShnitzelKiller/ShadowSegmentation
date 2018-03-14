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
    virtual void SetUniforms();
    virtual void GetShader();
    GLuint textureID;
};


#endif //RENDERER_SCREENSPACEQUAD_H
