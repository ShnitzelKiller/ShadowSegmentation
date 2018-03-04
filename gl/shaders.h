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
    Program();
    void AttachVertexShader(const char* vshader);
    void AttachFragmentShader(const char* fshader);
    void AttachShaders(const char* vshader, const char* fshader);
    void Link();
    void Use();
    void Unuse();
    GLint GetAttributeLocation(const char* name);
    GLint GetUniformLocation(const char* name);
    void BindAttribLocation(GLuint index, const char* name);
    ~Program();
private:
    void DeleteShaders();
    GLuint program_;
    GLuint vshader_;
    GLuint fshader_;
};

#endif //RENDERER_SHADERS_H