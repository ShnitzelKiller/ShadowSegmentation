//
// Created by James Noeckel on 2/20/18.
//

#include <iostream>
#include "Renderer.h"
#include <glm/gtx/transform.hpp>
#include "../mathdebug.h"
#include "../gl/shaders.h"



Renderer::Renderer(Scene *scene, int width, int height) {

    this->width = width;
    this->height = height;
    this->scene = scene;
    num_buffers = (int) scene->lights.size();

    images = new GLuint[num_buffers];
    glGenTextures(num_buffers, images);

    framebuffers = new GLuint[num_buffers];
    glGenFramebuffers(num_buffers, framebuffers);

    //create shaders
    program = new Program();
    program->AttachShaders(vertexSource, fragSource);
    program->Link();

    mvp_uniform = program->GetUniformLocation("mvp");
    nmw_uniform = program->GetUniformLocation("nmw");


    //create framebuffers
    for (int i=0; i<num_buffers; i++) {
        glBindTexture(GL_TEXTURE_2D, images[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glBindTexture(GL_TEXTURE_2D, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, framebuffers[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, images[i], 0);
        GLenum fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (fboStatus != GL_FRAMEBUFFER_COMPLETE) {
            fprintf(stderr, "glCheckFramebufferStatus: %x\n", fboStatus);
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    program->Unuse();
}

void Renderer::Render() {
    program->Use();
    for (size_t i=0; i<num_buffers; i++) {

        glBindFramebuffer(GL_FRAMEBUFFER, framebuffers[i]);
        GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
        glDrawBuffers(1, DrawBuffers);
        glViewport(0, 0, width, height);
        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glm::mat4 VP = scene->GetProjectionMatrix(i);

        for (auto it = scene->instances.begin(); it != scene->instances.end(); it++) {
            Mesh m = scene->meshes[it->meshID];
            std::cout << "drawing instance " << m.name << " (" << m.IndexCount << " indices)" << std::endl;
            glm::mat4 MW;

            MW = translate(-it->RotationOrigin) * MW;
            MW = mat4_cast(it->Rotation) * MW;
            MW = translate(it->RotationOrigin) * MW;
            MW = scale(it->Scale) * MW;
            MW = translate(it->Translation) * MW;
            glm::mat4 MVP = VP * MW;
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
    program->Unuse();
}

Renderer::~Renderer() {
    glDeleteTextures(num_buffers, images);
    glDeleteFramebuffers(num_buffers, framebuffers);
    delete program;
    program = nullptr;
    delete images;
    delete framebuffers;
}

GLuint *Renderer::GetImages(size_t *length) {
    *length = (size_t) num_buffers;
    return images;
}
