#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "scene/Scene.h"
#include "scene/Renderer.h"
#include "../shaders.h"
#include <chrono>
#include <thread>

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

Scene *s;
Renderer *r;
Program *program;


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
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible.\n" );
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

    //get framebuffer size
    int fwidth, fheight;
    glfwGetFramebufferSize(window, &fwidth, &fheight);

    std::cout << "framebuffer: " << fwidth << " x " << fheight << std::endl;

    //build scene
    s = new Scene(-10, -10, 10, 10);
    size_t torusMeshID = s->LoadMesh("../data/torus.obj");
    size_t triMeshID = s->AddTri(-1.1f, -1.1f, 0.5f, 1.1f, -1.1f, 0.5f, -1.1f, 1.1f, 0.5f);
    s->AddInstance(torusMeshID, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0,0,0), glm::angleAxis(0.f, glm::vec3(0.f, 1.f, 0.f)), glm::vec3(0,0,0));
    s->AddInstance(triMeshID);
    s->AddDirectionalLight(glm::vec3(0, 0, -1));
    r = new Renderer(s, fwidth, fheight);
    r->Render();
    size_t n;
    GLuint *textures = r->GetImages(&n);

    //render to fullscreen quad

    program = new Program(simpleVertexShader, simpleFragShader);
    program->Use();

    GLint posAttrib = program->GetAttributeLocation("pos");
    GLint texAttrib = program->GetAttributeLocation("tex");
    GLint texUniform = program->GetUniformLocation("image");

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

    glActiveTexture(GL_TEXTURE0);

    //debug texture
    /*float pixels[] = {
            0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f
    };
    GLuint test_tex;
    glGenTextures(1, &test_tex);
    glBindTexture(GL_TEXTURE_2D, test_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 3, 2, 0, GL_RGB, GL_FLOAT, pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);*/

    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glUniform1i(texUniform, 0);

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
    glViewport(0, 0, fwidth, fheight);

    do{
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
        std::this_thread::sleep_for(std::chrono::milliseconds(34));
    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );

    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &tbo);
    glDeleteBuffers(1, &ebo);
    glDeleteVertexArrays(1, &vao);
    program->Unuse();
    delete program;
    delete s;
    delete r;

    GLuint err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        fprintf(stderr, "error %x", err);
    }

    return 0;
}