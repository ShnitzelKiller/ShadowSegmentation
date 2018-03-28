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
    /**
     * Attach a vertex shader from source
     * @param vertexShader vertex shader source code
     */
    void AttachVertexShader(const char* vertexShader);
    /**
     * Attach a fragment shader from source
     * @param fragShader
     */
    void AttachFragmentShader(const char* fragShader);
    /**
     * Attach a vertex and fragment shader from source
     * @param vertexShader
     * @param fragShader
     */
    void AttachShaders(const char* vertexShader, const char* fragShader);
    /**
     * Link the program and delete the shader objects
     */
    void Link();
    /**
     * Make this the active program
     */
    void Use();
    /**
     * Clear the active program
     */
    void Unuse();
    /**
     * Get the location of the specified attribute
     * @param name
     * @return attribute index, or -1 if not found
     */
    GLint GetAttributeLocation(const char* name);
    /**
     * Get the location of the specified uniform
     * @param name
     * @return uniform index, or -1 if not found
     */
    GLint GetUniformLocation(const char* name);
    /**
     * Set the index of the specified attribute
     * @param index
     * @param name
     */
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
