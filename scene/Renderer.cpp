//
// Created by James Noeckel on 2/20/18.
//

#include <iostream>
#include "Renderer.h"
#include <glm/gtx/transform.hpp>
#include "../mathdebug.h"
#include "../shaders.h"



void Renderer::Init(Scene *scene, int width, int height) {

    this->width = width;
    this->height = height;
    this->scene = scene;
    num_buffers = (int) scene->lights.size();

    images = new GLuint[num_buffers];
    glGenTextures(num_buffers, images);

    framebuffers = new GLuint[num_buffers];
    glGenFramebuffers((int) num_buffers, framebuffers);

    //create shaders
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, nullptr);
    glCompileShader(vertexShader);
    GLint status;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        char buffer[512];
        glGetShaderInfoLog(vertexShader, 512, nullptr, buffer);
        std::cerr << "vertex shader compilation failed: " << std::string(buffer) << std::endl;
    }

    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragSource, nullptr);
    glCompileShader(fragShader);
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        char buffer[512];
        glGetShaderInfoLog(fragShader, 512, nullptr, buffer);
        std::cerr << "frag shader compilation failed: " << std::string(buffer) << std::endl;
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);
    if (status != GL_TRUE) {
        std::cerr << "linking failed" << std::endl;
    }


    mvp_uniform = glGetUniformLocation(shaderProgram, "mvp");
    nmw_uniform = glGetUniformLocation(shaderProgram, "nmw");


    //create framebuffers
    for (int i=0; i<num_buffers; i++) {
        glBindTexture(GL_TEXTURE_2D, images[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_BYTE, nullptr);
        glBindTexture(GL_TEXTURE_2D, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, framebuffers[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, images[i], 0);
        GLenum fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (fboStatus != GL_FRAMEBUFFER_COMPLETE) {
            fprintf(stderr, "glCheckFramebufferStatus: %x\n", fboStatus);
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    glUseProgram(0);
}

void Renderer::Render() {
    glUseProgram(shaderProgram);

    for (int i=0; i<num_buffers; i++) {

        glBindFramebuffer(GL_FRAMEBUFFER, framebuffers[i]);
        GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
        glDrawBuffers(1, DrawBuffers);
        glViewport(0, 0, width, height);
        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        glm::mat4 VP = scene->lights[i]->GetProjectionMatrix(scene->xmin, scene->ymin, scene->xmax, scene->ymax);

        for (auto it = scene->instances.begin(); it != scene->instances.end(); it++) {
            Mesh m = scene->meshes[it->meshID];
            std::cout << "drawing instance " << m.name << " (" << m.IndexCount << " indices)" << std::endl;
            glm::mat4 MW;
            MW = translate(-it->RotationOrigin) * MW;
            MW = mat4_cast(it->Rotation) * MW;
            MW = translate(it->RotationOrigin) * MW;
            MW = scale(it->Scale) * MW;
            MW = translate(it->Translation) * MW;
//            glm::mat4 MVP = VP * MW;
            glm::mat4 MVP;
            glm::mat3 NMW;
            NMW = glm::mat3_cast(it->Rotation) * NMW;
            NMW = glm::mat3(scale(1.0f/(it->Scale))) * NMW;
            printmat4(MVP);
            printmat3(NMW);

            glUniformMatrix4fv(mvp_uniform, 1, GL_FALSE, &MVP[0][0]);
            glUniformMatrix3fv(nmw_uniform, 1, GL_FALSE, &NMW[0][0]);
            glBindVertexArray(m.MeshVAO);
            glDrawElements(GL_TRIANGLES, m.IndexCount, GL_UNSIGNED_INT, (void*) 0);
            glBindVertexArray(0);
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    glUseProgram(0);
}

void Renderer::Destroy() {
    glDeleteTextures(num_buffers, images);
    glDeleteFramebuffers(num_buffers, framebuffers);
    glDeleteProgram(shaderProgram);
    delete images;
    delete framebuffers;
}

GLuint *Renderer::GetImages(size_t *length) {
    *length = (size_t) num_buffers;
    return images;
}
