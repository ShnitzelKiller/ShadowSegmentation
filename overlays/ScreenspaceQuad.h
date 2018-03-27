//
// Created by James Noeckel on 3/6/18.
//

#ifndef RENDERER_SCREENSPACEQUAD_H
#define RENDERER_SCREENSPACEQUAD_H


#include <GL/glew.h>
#include "../gl/shaders.h"

class ScreenspaceQuad {
public:
    ScreenspaceQuad();
    ScreenspaceQuad(ScreenspaceQuad &&other) noexcept;
    virtual void Render();
    virtual ~ScreenspaceQuad();
    void Init();
private:
    GLuint vao;
    GLuint vbo;
    GLuint tbo;
    GLuint ebo;
protected:
    std::shared_ptr<Program> program;
    GLint texUniform;
    virtual void GetUniforms() = 0;
    virtual void SetUniforms() = 0;
    virtual void GetShader() = 0;
};


#endif //RENDERER_SCREENSPACEQUAD_H
