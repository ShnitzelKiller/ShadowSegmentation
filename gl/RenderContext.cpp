//
// Created by James Noeckel on 3/27/18.
//

#include "RenderContext.h"

RenderContext::RenderContext(std::shared_ptr<Program> p) {
    this->p = std::move(p);
}

GLint RenderContext::SetUniform(std::string const &name, float val) {
    GLint pos = p->GetUniformLocation(name.c_str());
    floatMapping[pos] = val;
    return pos;
}

GLint RenderContext::SetUniform(std::string const &name, glm::mat4x4 val) {
    GLint pos = p->GetUniformLocation(name.c_str());
    mat4Mapping[pos] = val;
    return pos;
}

GLint RenderContext::SetUniform(std::string const &name, glm::mat3x3 val) {
    GLint pos = p->GetUniformLocation(name.c_str());
    mat3Mapping[pos] = val;
    return pos;
}

GLint RenderContext::SetUniform(std::string const &name, glm::vec2 val) {
    GLint pos = p->GetUniformLocation(name.c_str());
    vec2Mapping[pos] = val;
    return pos;
}

GLint RenderContext::SetUniform(std::string const &name, glm::vec3 val) {
    GLint pos = p->GetUniformLocation(name.c_str());
    vec3Mapping[pos] = val;
    return pos;
}

GLint RenderContext::SetUniform(std::string const &name, glm::vec4 val) {
    GLint pos = p->GetUniformLocation(name.c_str());
    vec4Mapping[pos] = val;
    return pos;
}

void RenderContext::Render(GLuint vao, GLuint indexCount) {
    p->Use();
    for (auto pair : floatMapping) {
        glUniform1f(pair.first, pair.second);
    }
    for (auto pair : mat4Mapping) {
        glUniformMatrix4fv(pair.first, 1, GL_FALSE, &pair.second[0][0]);
    }
    for (auto pair : mat3Mapping) {
        glUniformMatrix3fv(pair.first, 1, GL_FALSE, &pair.second[0][0]);
    }
    for (auto pair : vec2Mapping) {
        glUniform2fv(pair.first, 1, &pair.second[0]);
    }
    for (auto pair : vec3Mapping) {
        glUniform3fv(pair.first, 1, &pair.second[0]);
    }
    for (auto pair : vec4Mapping) {
        glUniform4fv(pair.first, 1, &pair.second[0]);
    }
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, (void*) 0);
    glBindVertexArray(0);
    p->Unuse();
}


