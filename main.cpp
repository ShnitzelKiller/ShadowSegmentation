#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "scene/Scene.h"
#include "scene/Renderer.h"
#include "gl/shaders.h"
#include "ScreenspaceQuad.h"
#include "ParallelSceneRenderer.h"
#include "BasicQuad.h"
#include "DilateQuad.h"
#include <chrono>
#include <thread>
#include <opencv/cv.h>

#define SCREEN_WIDTH 500
#define SCREEN_HEIGHT 500
#define RENDER_WIDTH 250
#define RENDER_HEIGHT 250


int main(int argc, char** argv) {
    std::cout << std::string(argv[0]) << std::endl;

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

    auto *pr = new ParallelSceneRenderer(-6, -6, 6, 6, RENDER_WIDTH, RENDER_HEIGHT, GL_RGBA);
    size_t cubeMeshID = pr->LoadMesh("../../data/cube.obj","../../data/cube_noise.obj");
    pr->AddInstance(cubeMeshID, glm::vec3(1, 1, 1), glm::vec3(0,0,0), glm::angleAxis(0.f, glm::vec3(1.f, 0.f, 0.f)), glm::vec3(0,0,1));
    pr->AddInstance(cubeMeshID, glm::vec3(1, 1, 1), glm::vec3(0,0,0), glm::angleAxis((float) M_PI/4, glm::vec3(1.f, 0.f, 0.f)), glm::vec3(1,1,2));
    auto *dirLight = new DirectionalLight(-1, -1, -1);
    auto *light = new PointLight(3, 3, 3);
    pr->AddLight(light);
    pr->AddLight(dirLight);

    std::vector<GLuint> textures1;
    std::vector<GLuint> textures2;
    pr->GetImages(textures1, textures2);


    auto *quad = new BasicQuad();
    quad->Init();

    auto *dilate = new DilateQuad(RENDER_WIDTH, RENDER_HEIGHT);
    dilate->Init();

    //rendering

    float ang = 0;

    std::cout << "render loop" << std::endl;
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    //initialize test image
//    GLuint temp;
//    glGenTextures(1, &temp);
//    glBindTexture(GL_TEXTURE_2D, temp);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, RENDER_WIDTH, RENDER_HEIGHT, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    auto *rtdiff = new RenderTexture(RENDER_WIDTH, RENDER_HEIGHT, 1, GL_RGBA);
    auto *rtint = new RenderTexture(RENDER_WIDTH, RENDER_HEIGHT, 1, GL_RGBA);

    do{

        //run renderer
        ang += 0.02f;
        dirLight->dir = glm::vec3(-cos(ang), -sin(ang), -1);
        light->pos = glm::vec3(2.1*cos(ang), 2.1*sin(ang), 5.2);
        pr->Render();

        //show results
        glClearColor(0, 1, 0, 1);
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

        //Composite real and fake to remove partial shadow
        {

            //draw intersection of synthetic shadow groups
            pr->SetVisible2(1, false);
            pr->Render2();

            rtint->Clear(0,0,0,0);
            rtint->Bind();
            glViewport(0,0,RENDER_WIDTH, RENDER_HEIGHT);
            quad->SetImage(textures2[0]);
            quad->Render();
            rtint->Unbind();
            pr->SetVisible2(1, true);
            pr->SetVisible2(0, false);
            pr->Render2();
            rtint->Bind();
            glBlendEquation(GL_FUNC_ADD);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_BLEND);
            dilate->SetImage(textures2[0]);
            dilate->SetRadius(4);
            dilate->Render();
            glDisable(GL_BLEND);
            rtint->Unbind();


            //add above shape to original shadow map with synthetic shadow mask removed
            rtdiff->Clear(0,0,0,0);
            rtdiff->Bind();
            glViewport(0,0,RENDER_WIDTH, RENDER_HEIGHT);
            dilate->SetImage(textures2[0]);

            dilate->SetInvert(true);
            dilate->SetRadius(5);
            dilate->Render();
            dilate->SetInvert(false);

            glBlendEquation(GL_FUNC_ADD);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_BLEND);
            quad->SetImage(textures1[0]);
            quad->Render();
            quad->SetImage(rtint->GetTextureIDs()[0]);
            glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);
            quad->Render();
            rtdiff->Unbind();
            glDisable(GL_BLEND);

            pr->SetVisible2(0, true);
            pr->Render2();

            //display processed image as a test
            glViewport(fwidth/4, fheight/4, fwidth/2 ,fheight/2);
            quad->SetImage(rtdiff->GetTextureIDs()[0]);
            quad->Render();
        }



        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );
    //delete test image
//    glBindTexture(GL_TEXTURE_2D, 0);
//    glDeleteTextures(1, &temp);


    delete rtdiff;
    delete rtint;
    delete quad;
    delete dilate;
    delete light;
    delete dirLight;
    delete pr;

    Program::DestroyShaders();

    GLuint err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        fprintf(stderr, "error %x before loop", err);
    }
    std::cout << "exiting" << std::endl;
    return 0;
}