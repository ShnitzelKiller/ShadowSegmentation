//
// Created by James Noeckel on 2/20/18.
//
#define GLM_ENABLE_EXPERIMENTAL

#include <iostream>
#include "Renderer.h"
#include <glm/gtx/transform.hpp>

Renderer::Renderer(Scene *scene, int width, int height) : scene(scene), width(width), height(height) {
    num_buffers = (int) scene->lights.size();

    rendertextures = std::vector<RenderTexture>();
    for (int i=0; i<num_buffers; i++) {
        RenderTexture rt(width, height);
        rendertextures.push_back(std::move(rt));
    }

    //create shaders
    program = new Program();
    program->AttachShaders(vertexSource, fragSource);
    program->Link();

    mvp_uniform = program->GetUniformLocation("mvp");
    nmw_uniform = program->GetUniformLocation("nmw");
}

void Renderer::Render() {
    program->Use();
    for (size_t i=0; i<num_buffers; i++) {

        rendertextures[i].Bind();
        glViewport(0, 0, width, height);
        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glm::mat4 VP = scene->GetProjectionMatrix(i);

        for (auto it = scene->instances.begin(); it != scene->instances.end(); it++) {
            if (!it->active) continue;
            Mesh m = scene->meshes[it->meshID];
            //std::cout << "drawing instance " << m.name << " (" << m.IndexCount << " indices)" << std::endl;
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

            glUniformMatrix4fv(mvp_uniform, 1, GL_FALSE, &MVP[0][0]);
            glUniformMatrix3fv(nmw_uniform, 1, GL_FALSE, &NMW[0][0]);
            glBindVertexArray(m.MeshVAO);
            glDrawElements(GL_TRIANGLES, m.IndexCount, GL_UNSIGNED_INT, (void*) 0);
            glBindVertexArray(0);
        }

        rendertextures[i].Unbind();
    }
    program->Unuse();
}

Renderer::~Renderer() {
    delete program;
}

std::vector<GLuint> Renderer::GetImages() {
    std::vector<GLuint> images;
    for (int i=0; i<num_buffers; i++) {
        images.push_back(rendertextures[i].GetTextureIDs()[0]);
    }
    return images;
}
