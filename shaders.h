//
// Created by James Noeckel on 2/28/18.
//

#ifndef RENDERER_SHADERS_H
#define RENDERER_SHADERS_H
#include <GL/glew.h>

extern const char* fragSource;

extern const char* vertexSource;

extern const char* simpleVertexShader;

extern const char* simpleFragShader;

class Program {
public:
    void CreateFromShaders(const char* vert, const char* frag);
    void Destroy();
    void Use();
    void Unuse();
    GLint GetAttributeLocation(const char* name);
    GLint GetUniformLocation(const char* name);
private:
    GLuint program_;
    GLuint vshader_;
    GLuint fshader_;
};

#endif //RENDERER_SHADERS_H
