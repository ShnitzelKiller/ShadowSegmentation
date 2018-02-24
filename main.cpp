#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "scene/Scene.h"
#include "scene/Renderer.h"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

const char* vertexShader = R"glsl(
    #version 150 core

    in vec2 pos;
    in vec2 tex;

    out vec2 T;

    void main() {
        T = tex;
        gl_Position = vec4(pos, 0.0, 1.0);
    }
)glsl";

const char* fragShader = R"glsl(
    #version 150 core

    uniform sampler2D image;
    in vec2 T;
    out vec4 fragColor;

    void main() {
        fragColor = texture(image, T);
    }
)glsl";

Scene *s;
Renderer *r;


int main() {

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL

    GLFWwindow* window; // (In the accompanying source code, this variable is global for simplicity)
    window = glfwCreateWindow( SCREEN_WIDTH, SCREEN_HEIGHT, "Display", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); // Initialize GLEW
    glewExperimental=true; // Needed in core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    s = new Scene(-10, -10, 10, 10);
    s->LoadMesh("../data/torus.obj");
    size_t torusID;
    s->AddInstance(0, &torusID);
    s->AddDirectionalLight(glm::vec3(0, 0, -1));
    r = new Renderer();
    r->Init(s, SCREEN_WIDTH, SCREEN_HEIGHT);
    r->Render();
    size_t n;
    GLuint *textures = r->GetImages(&n);

    //create fullscreen quad

    GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vshader, 1, &vertexShader, nullptr);
    glCompileShader(vshader);
    GLint status;
    glGetShaderiv(vshader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        char buffer[512];
        glGetShaderInfoLog(vshader, 512, nullptr, buffer);
        std::cerr << std::string(buffer) << std::endl;
        return -1;
    }
    GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fshader, 1, &fragShader, nullptr);
    glCompileShader(fshader);
    glGetShaderiv(fshader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        char buffer[512];
        glGetShaderInfoLog(fshader, 512, nullptr, buffer);
        std::cerr << std::string(buffer) << std::endl;
        return -1;
    }
    GLuint program = glCreateProgram();
    glAttachShader(program, fshader);
    glAttachShader(program, vshader);
    glLinkProgram(program);
    glUseProgram(program);

    GLint posAttrib = glGetAttribLocation(program, "pos");
    GLint texAttrib = glGetAttribLocation(program, "tex");
    GLint texUniform = glGetUniformLocation(program, "image");

    const float verts[] = {-1, 1,
                            1, 1,
                           -1, -1,
                            1, -1};

    const float texCoords[] = {
            0, 1,
            1, 1,
            0, 0,
            1, 0
    };

    const GLuint indices[] = {
            3, 1, 0,
            3, 0, 2
    };

    //debug texture
    /*float pixels[] = {
            0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f
    };
    GLuint test_tex;
    glGenTextures(1, &test_tex); */

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glUniform1i(texUniform, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(posAttrib);

    GLuint tbo;
    glGenBuffers(1, &tbo);
    glBindBuffer(GL_ARRAY_BUFFER, tbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(texAttrib);

    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glClearColor(0, 0, 0, 0);
//    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    do{
        // TODO: Draw the stuff
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );

    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &tbo);
    glDeleteBuffers(1, &ebo);
    glDeleteVertexArrays(1, &vao);
    glDeleteProgram(program);
    glDeleteShader(vshader);
    glDeleteShader(fshader);

    delete s;
    r->Destroy();
    delete r;

    return 0;
}