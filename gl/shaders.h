//
// Created by James Noeckel on 2/28/18.
//

#ifndef RENDERER_SHADERS_H
#define RENDERER_SHADERS_H
#include <GL/glew.h>
#include <memory>

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
    static std::shared_ptr<Program> GetDefaultShader();
    static std::shared_ptr<Program> GetSimpleShader();
    static std::shared_ptr<Program> GetDilateShader();
    static std::shared_ptr<Program> GetPointLitShader();
    static std::shared_ptr<Program> GetDirLitShader();
private:
    void DeleteShaders();
    GLuint program_;
    GLuint vshader_;
    GLuint fshader_;
    static std::shared_ptr<Program> defaultShader;
    static std::shared_ptr<Program> simpleShader;
    static std::shared_ptr<Program> dilateShader;
    static std::shared_ptr<Program> pointLitShader;
    static std::shared_ptr<Program> dirLitShader;
};

#endif //RENDERER_SHADERS_H
