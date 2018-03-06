#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "scene/Scene.h"
#include "scene/Renderer.h"
#include "gl/shaders.h"
#include <chrono>
#include <thread>
#include <math.h>

#define SCREEN_WIDTH 500
#define SCREEN_HEIGHT 500


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
    std::cout << "initializing" << std::endl;

    //build scene
    auto *s = new Scene(-4, -4, 4, 4);
    size_t roughCubeMeshID = s->LoadMesh("../data/cube_noise.obj");
    s->AddInstance(roughCubeMeshID, glm::vec3(1, 1, 1), glm::vec3(0,0,0), glm::angleAxis(0.f, glm::vec3(1.f, 0.f, 0.f)), glm::vec3(-.5f,-.5f,1));
    s->AddInstance(roughCubeMeshID, glm::vec3(1, 1, 1), glm::vec3(0,0,0), glm::angleAxis(0.f, glm::vec3(1.f, 0.f, 0.f)), glm::vec3(.5f,.5f,2));
    //s->AddInstance(roughCubeMeshID, glm::vec3(1, 1, 1), glm::vec3(0,0,0), glm::angleAxis(0.f, glm::vec3(1.f, 0.f, 0.f)), glm::vec3(0,0,0));
    auto *light = new DirectionalLight(-1, -1, -1);
    s->AddLight(light);
    auto *r = new Renderer(s, fwidth, fheight);
    r->Render();
    std::vector<GLuint> textures = r->GetImages();
    //render to fullscreen quad
    std::cout << "render result to fullscreen quad" << std::endl;

    auto *program = new Program();
    program->AttachShaders(simpleVertexShader, simpleFragShader);
    program->Link();

    /*GLint posAttrib = program->GetAttributeLocation("pos");
    GLint texAttrib = program->GetAttributeLocation("tex");*/

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
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    GLuint tbo;
    glGenBuffers(1, &tbo);
    glBindBuffer(GL_ARRAY_BUFFER, tbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBindVertexArray(0);

    //rendering

    glClearColor(0, 0, 0, 1);
    glViewport(0, 0, fwidth, fheight);

    float ang = 0;

    std::cout << "render loop" << std::endl;

    do{
        ang += 0.02f;
        light->dir = glm::vec3(cos(ang), sin(ang), -1);
        r->Render();

        program->Use();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
        std::this_thread::sleep_for(std::chrono::milliseconds(34));
    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );

    GLuint err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        fprintf(stderr, "error %x 1", err);
    }

    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &tbo);
    glDeleteBuffers(1, &ebo);
    glDeleteVertexArrays(1, &vao);
    program->Unuse();

    delete program;
    while ((err = glGetError()) != GL_NO_ERROR) {
        fprintf(stderr, "error %x", err);
    }
    std::cout << "deleting scene & lights" << std::endl;
    delete s;
    delete light;
    std::cout << "deleting renderer" << std::endl;
    delete r;

    std::cout << "exiting" << std::endl;
    return 0;
}