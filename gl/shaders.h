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

extern const char* dilateFragShader;

extern const char* pointLitFragShader;

class Program {
public:
    Program();
    Program(Program &&other) noexcept;
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
    static Program *GetDefaultShader();
    static Program *GetSimpleShader();
    static Program *GetDilateShader();
    static Program *GetPointLitShader();
    static Program *GetDirLitShader();
    static void DestroyShaders();
private:
    void DeleteShaders();
    GLuint program_;
    GLuint vshader_;
    GLuint fshader_;
    static Program *defaultShader;
    static Program *simpleShader;
    static Program *dilateShader;
    static Program *pointLitShader;
    static Program *dirLitShader;
};

#endif //RENDERER_SHADERS_H
