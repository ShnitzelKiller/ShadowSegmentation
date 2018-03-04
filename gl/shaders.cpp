//
// Created by James Noeckel on 2/28/18.
//

#include "shaders.h"
#include <iostream>
#include <string>

const char* fragSource = R"glsl(
    #version 330 core

    out vec4 outColor;
    in vec2 T;
    in vec3 N;

    void main()
    {
        outColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
)glsl";

const char* vertexSource = R"glsl(
#version 330 core

uniform mat4 mvp;
uniform mat3 nmw;

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

out vec2 T;
out vec3 N;

void main()
{
    N = normalize(nmw * normal);
    T = texCoord;
    gl_Position = mvp * vec4(position, 1.0);
})glsl";

const char* simpleVertexShader = R"glsl(
    #version 330 core

    layout(location = 0) in vec2 pos;
    layout(location = 1) in vec2 tex;

    out vec2 T;

    void main() {
        T = tex;
        gl_Position = vec4(pos, 0.0, 1.0);
    }
)glsl";

const char* simpleFragShader = R"glsl(
    #version 330 core

    uniform sampler2D image;
    in vec2 T;
    out vec4 fragColor;

    void main() {
        fragColor = texture(image, T) + vec4(0, T * 0.5f, 0);
    }
)glsl";

Program::Program() : vshader_(0), fshader_(0) {
    program_ = glCreateProgram();
}

void Program::AttachVertexShader(const char *vert) {
    GLuint newvert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(newvert, 1, &vert, nullptr);
    glCompileShader(newvert);
    GLint status;
    glGetShaderiv(newvert, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        char buffer[512];
        glGetShaderInfoLog(newvert, 512, nullptr, buffer);
        std::cerr << "vertex shader compilation failed: " << std::string(buffer) << std::endl;
        return;
    }
    glAttachShader(program_, newvert);
    vshader_ = newvert;
}

void Program::AttachFragmentShader(const char *frag) {
    GLint status;
    GLuint newfrag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(newfrag, 1, &frag, nullptr);
    glCompileShader(newfrag);
    glGetShaderiv(newfrag, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        char buffer[512];
        glGetShaderInfoLog(newfrag, 512, nullptr, buffer);
        std::cerr << "fragment shader compilation failed: " << std::string(buffer) << std::endl;
        return;
    }
    glAttachShader(program_, newfrag);
    fshader_ = newfrag;
}

void Program::AttachShaders(const char *vert, const char *frag) {

    AttachVertexShader(vert);
    AttachFragmentShader(frag);
}

void Program::Link() {
    GLint status;
    glLinkProgram(program_);
    glGetProgramiv(program_, GL_LINK_STATUS, &status);
    if (status != GL_TRUE) {
        std::cerr << "linking failed" << std::endl;
        return;
    }
    DeleteShaders();
}

Program::~Program() {
    DeleteShaders();
    glDeleteProgram(program_);
}

void Program::Use() {
    glUseProgram(program_);
}

void Program::Unuse() {
    glUseProgram(0);
}

GLint Program::GetAttributeLocation(const char *name) {
    return glGetAttribLocation(program_, name);
}

GLint Program::GetUniformLocation(const char *name) {
    return glGetUniformLocation(program_, name);
}

void Program::BindAttribLocation(GLuint index, const char *name) {
    glBindAttribLocation(program_, index, name);
}

void Program::DeleteShaders() {
    if (vshader_ != 0) {
        glDetachShader(program_, vshader_);
        glDeleteShader(vshader_);
        vshader_ = 0;
    }
    if (fshader_ != 0) {
        glDetachShader(program_, fshader_);
        glDeleteShader(fshader_);
        fshader_ = 0;
    }
}

