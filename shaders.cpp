//
// Created by James Noeckel on 2/28/18.
//

#include "shaders.h"

const char* fragSource = R"glsl(
    #version 150 core

    out vec4 outColor;
    in vec2 T;
    in vec3 N;

    void main()
    {
        outColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
)glsl";

const char* vertexSource = R"glsl(
#version 150 core

uniform mat4 mvp;
uniform mat3 nmw;

in vec3 position;
in vec2 texCoord;
in vec3 normal;

out vec2 T;
out vec3 N;

void main()
{
    N = normalize(nmw * normal);
    T = texCoord;
    gl_Position = mvp * vec4(position, 1.0);
})glsl";

const char* simpleVertexShader = R"glsl(
    #version 150 core

    in vec2 pos;
    in vec2 tex;

    out vec2 T;

    void main() {
        T = tex;
        gl_Position = vec4(pos, 0.0, 1.0);
    }
)glsl";

const char* simpleFragShader = R"glsl(
    #version 150 core

    uniform sampler2D image;
    in vec2 T;
    out vec4 fragColor;

    void main() {
        fragColor = texture(image, T);
    }
)glsl";