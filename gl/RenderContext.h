//
// Created by James Noeckel on 3/27/18.
//

#ifndef RENDERER_RENDERCONTEXT_H
#define RENDERER_RENDERCONTEXT_H


#include <map>
#include <string>
#include <glm/mat4x4.hpp>
#include "shaders.h"

class RenderContext {
public:
    explicit RenderContext(Program *p);
    GLint SetUniform(std::string const &name, float val);
    GLint SetUniform(std::string const &name, glm::mat4x4 val);
    GLint SetUniform(std::string const &name, glm::mat3x3 val);
    GLint SetUniform(std::string const &name, glm::vec2 val);
    GLint SetUniform(std::string const &name, glm::vec3 val);
    GLint SetUniform(std::string const &name, glm::vec4 val);


    void Render(GLuint vao, GLuint indexCount);
private:
    Program *p;
    std::map<GLint, float> floatMapping;
    std::map<GLint, glm::mat4x4> mat4Mapping;
    std::map<GLint, glm::mat3x3> mat3Mapping;
    std::map<GLint, glm::vec2> vec2Mapping;
    std::map<GLint, glm::vec3> vec3Mapping;
    std::map<GLint, glm::vec4> vec4Mapping;
};


#endif //RENDERER_RENDERCONTEXT_H
