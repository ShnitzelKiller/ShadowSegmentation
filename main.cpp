#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "scene/Scene.h"
#include "scene/Renderer.h"
#include "gl/shaders.h"
#include "ScreenspaceQuad.h"
#include "ParallelSceneRenderer.h"
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

    GLFWwindow* window;
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
    std::cout << "loading scene..." << std::endl;

    //build scene

    auto *pr = new ParallelSceneRenderer(-6, -6, 6, 6, 250, 250);
    size_t cubeMeshID = pr->LoadMesh("../data/cube.obj","../data/cube_noise.obj");
    pr->AddInstance(cubeMeshID, glm::vec3(1, 1, 1), glm::vec3(0,0,0), glm::angleAxis(0.f, glm::vec3(1.f, 0.f, 0.f)), glm::vec3(0,0,1));
    pr->AddInstance(cubeMeshID, glm::vec3(1, 1, 1), glm::vec3(0,0,0), glm::angleAxis((float) M_PI/4, glm::vec3(1.f, 0.f, 0.f)), glm::vec3(1,1,2));
    auto *dirLight = new DirectionalLight(-1, -1, -1);
    auto *light = new PointLight(3, 3, 3);
    pr->AddLight(light);
    pr->AddLight(dirLight);


    std::vector<GLuint> textures1;
    std::vector<GLuint> textures2;
    pr->GetImages(textures1, textures2);

    auto *quad = new ScreenspaceQuad();

    //rendering

    float ang = 0;

    std::cout << "render loop" << std::endl;
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    do{
        //run renderer
        ang += 0.02f;
        dirLight->dir = glm::vec3(-cos(ang), -sin(ang), -1);
        light->pos = glm::vec3(2.1*cos(ang), 2.1*sin(ang), 4.2);
        pr->Render();

        //show results
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glViewport(0, fheight/2+1, fwidth/2-1, fheight/2-1);
        quad->SetImage(textures1[0]);
        quad->Render();

        glViewport(0, 0, fwidth/2-1, fheight/2-1);
        quad->SetImage(textures1[1]);
        quad->Render();

        glViewport(fwidth/2+1, fheight/2+1, fwidth/2-1, fheight/2-1);
        quad->SetImage(textures2[0]);
        quad->Render();

        glViewport(fwidth/2+1, 0, fwidth/2-1, fheight/2-1);
        quad->SetImage(textures2[1]);
        quad->Render();

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
        std::this_thread::sleep_for(std::chrono::milliseconds(34));
    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );

    delete quad;
    delete light;
    delete dirLight;
    delete pr;
    Program::DestroyShaders();

    GLuint err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        fprintf(stderr, "error %x", err);
    }
    std::cout << "exiting" << std::endl;
    return 0;
}