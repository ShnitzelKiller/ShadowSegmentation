//
// Created by James Noeckel on 2/28/18.
//

#include "shaders.h"
#include <iostream>
#include <string>

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

const char* fragSource = R"glsl(
    #version 330 core

    out vec4 outColor;
    in vec2 T;
    in vec3 N;

    void main()
    {
        outColor = vec4(0.0, 0.0, 0.0, 0.0);
    }
)glsl";

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
    uniform float invert;
    in vec2 T;
    out vec4 fragColor;

    void main() {
        vec4 col = texture(image, T);
        fragColor = invert > 0.5 ? 1.0-col : col;
    }
)glsl";

const char* dilateFragShader = R"glsl(
    #version 330 core

    uniform sampler2D image;
    uniform int radius;
    uniform int dim;
    uniform int width;
    uniform int height;
    uniform float invert;

    in vec2 T;
    out vec4 fragColor;

    void main() {
        vec4 col = vec4(1, 1, 1, 1);
        vec2 wh = vec2(width, height);
        int i;
        vec2 disp = vec2(0, 0);
        for (i=-radius; i<=radius; i++) {
            disp[dim] = i;
            vec4 tempcol = texture(image, T + disp/wh);
            col = min(col, tempcol);
        }
        fragColor = invert > 0.5 ? 1.0-col : col;
    }
)glsl";

const char* pointLitFragShader = R"glsl(
    #version 330 core
    uniform vec3 pos;
    uniform vec3 intensity;
    uniform vec4 bounds;
    in vec2 T;
    out vec4 fragColor;
    void main() {
        vec3 o = vec3(T * (bounds.zw - bounds.xy) + bounds.xy, 0);
        vec3 l = pos - o;
        float invdist = 1.0/length(l);
        float fac = l.z * invdist;
        float finalfac = fac * dot(intensity, vec3(1.0, invdist, invdist*invdist));
        fragColor = vec4(finalfac, finalfac, finalfac, 1.0);
    }
)glsl";

const char* dirLitFragShader = R"glsl(
    #version 330 core
    uniform vec3 dir;
    uniform float intensity;
    in vec2 T;
    out vec4 fragColor;
    void main() {
        vec3 col = vec3(-intensity * dir.z / length(dir));
        fragColor = vec4(col, 1.0);
    }
)glsl";

Program::Program() : vshader_(0), fshader_(0) {
    program_ = glCreateProgram();
}

void Program::AttachVertexShader(const char *vertexShader) {
    GLuint newvert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(newvert, 1, &vertexShader, nullptr);
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

void Program::AttachFragmentShader(const char *fragShader) {
    GLint status;
    GLuint newfrag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(newfrag, 1, &fragShader, nullptr);
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

void Program::AttachShaders(const char *vertexShader, const char *fragShader) {

    AttachVertexShader(vertexShader);
    AttachFragmentShader(fragShader);
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
    std::cout << "deleting shader " << program_ << std::endl;
    DeleteShaders();
    glDeleteProgram(program_);
    glUseProgram(0);
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

Program::Program(Program &&other) noexcept : program_(other.program_), vshader_(other.vshader_), fshader_(other.fshader_) {
    other.program_ = 0;
    other.vshader_ = 0;
    other.fshader_ = 0;
}

//static stuff

std::shared_ptr<Program> Program::defaultShader = std::shared_ptr<Program>(nullptr);
std::shared_ptr<Program> Program::simpleShader = std::shared_ptr<Program>(nullptr);
std::shared_ptr<Program> Program::dilateShader = std::shared_ptr<Program>(nullptr);
std::shared_ptr<Program> Program::pointLitShader = std::shared_ptr<Program>(nullptr);
std::shared_ptr<Program> Program::dirLitShader = std::shared_ptr<Program>(nullptr);

std::shared_ptr<Program> Program::GetDefaultShader() {
    if (!defaultShader) {
        defaultShader = std::make_shared<Program>();
        defaultShader->AttachShaders(vertexSource, fragSource);
        defaultShader->Link();
    }
    return defaultShader;
}

std::shared_ptr<Program> Program::GetSimpleShader() {
    if (!simpleShader) {
        simpleShader = std::make_shared<Program>();
        simpleShader->AttachShaders(simpleVertexShader, simpleFragShader);
        simpleShader->Link();
    }
    return simpleShader;
}

std::shared_ptr<Program> Program::GetDilateShader() {
    if (!dilateShader) {
        dilateShader = std::make_shared<Program>();
        dilateShader->AttachShaders(simpleVertexShader, dilateFragShader);
        dilateShader->Link();
    }
    return dilateShader;
}

std::shared_ptr<Program> Program::GetPointLitShader() {
    if (!pointLitShader) {
        pointLitShader = std::make_shared<Program>();
        pointLitShader->AttachShaders(simpleVertexShader, pointLitFragShader);
        pointLitShader->Link();
    }
    return pointLitShader;
}

std::shared_ptr<Program> Program::GetDirLitShader() {
    if (!dirLitShader) {
        dirLitShader = std::make_shared<Program>();
        dirLitShader->AttachShaders(simpleVertexShader, dirLitFragShader);
        dirLitShader->Link();
    }
    return dirLitShader;
}



