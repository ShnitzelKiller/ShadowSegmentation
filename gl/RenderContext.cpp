//
// Created by James Noeckel on 3/27/18.
//

#include "RenderContext.h"

RenderContext::RenderContext(std::shared_ptr<Program> p) {
    this->p = p;
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
    for (auto it = floatMapping.begin(); it != floatMapping.end(); it++) {
        glUniform1f(it->first, it->second);
    }
    for (auto it = mat4Mapping.begin(); it != mat4Mapping.end(); it++) {
        glUniformMatrix4fv(it->first, 1, GL_FALSE, &it->second[0][0]);
    }
    for (auto it = mat3Mapping.begin(); it != mat3Mapping.end(); it++) {
        glUniformMatrix3fv(it->first, 1, GL_FALSE, &it->second[0][0]);
    }
    for (auto it = vec2Mapping.begin(); it != vec2Mapping.end(); it++) {
        glUniform2fv(it->first, 1, &it->second[0]);
    }
    for (auto it = vec3Mapping.begin(); it != vec3Mapping.end(); it++) {
        glUniform3fv(it->first, 1, &it->second[0]);
    }
    for (auto it = vec4Mapping.begin(); it != vec4Mapping.end(); it++) {
        glUniform4fv(it->first, 1, &it->second[0]);
    }
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, (void*) 0);
    glBindVertexArray(0);
    p->Unuse();
}


