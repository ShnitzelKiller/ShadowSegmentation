//
// Created by James Noeckel on 2/20/18.
//
#define GLM_ENABLE_EXPERIMENTAL

#include <iostream>
#include "Renderer.h"
#include <glm/gtx/transform.hpp>

Renderer::Renderer(Scene &scene, int width, int height, GLenum internalformat) : scene(scene), width(width), height(height), internalformat(internalformat), finalBuffer(width, height, 1, internalformat) {
    quad.Init();

    num_buffers = (int) scene.lights.size();

    rendertextures = std::vector<RenderTexture>();
    for (int i=0; i<num_buffers; i++) {
        RenderTexture rt(width, height, 1, internalformat);
        rendertextures.push_back(std::move(rt));
    }

    //create shaders
    program = Program::GetDefaultShader();

    mvp_uniform = program->GetUniformLocation("mvp");
    nmw_uniform = program->GetUniformLocation("nmw");
}

void Renderer::Render(bool shading) {
    Update();
    finalBuffer.Clear(0, 0, 0, 0);

    for (size_t i=0; i<num_buffers; i++) {
        rendertextures[i].Clear(1, 1, 1, 1);
        rendertextures[i].Bind();
        glViewport(0, 0, width, height);

        if (shading) {
            ScreenspaceQuad *surface = scene.GetLitSurface(i);
            surface->Render();
        }

        glm::mat4 VP = scene.GetProjectionMatrix(i);

        for (auto it = scene.instances.begin(); it != scene.instances.end(); it++) {
            if (!it->active) continue;
            program->Use();
            Mesh m = scene.meshes[it->meshID];
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
            program->Unuse();
        }

        rendertextures[i].Unbind();

        finalBuffer.Bind();
        quad.SetImage(rendertextures[i].GetTextureIDs()[0]);
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_ONE, GL_ONE);
        quad.Render();
        glDisable(GL_BLEND);
        finalBuffer.Unbind();
    }
}

std::vector<GLuint> Renderer::GetImages() {
    Update();
    std::vector<GLuint> images;
    for (int i=0; i<num_buffers; i++) {
        RenderTexture &curr = rendertextures[i];
        GLuint *tex = curr.GetTextureIDs();
        images.push_back(tex[0]);
    }
    return images;
}

void Renderer::Update() {
    for (int i=num_buffers; i<scene.lights.size(); i++) {
        RenderTexture rt(width, height, 1, internalformat);
        rendertextures.push_back(std::move(rt));
    }
    num_buffers = (int) scene.lights.size();
}

int Renderer::GetWidth() const {
    return width;
}

int Renderer::GetHeight() const {
    return height;
}

void Renderer::ReadImageData(void *buffer, GLenum format, GLenum type, int index) {
    rendertextures[index].Bind();
    glReadPixels(0, 0, width, height, format, type, buffer);
    rendertextures[index].Unbind();
}

void Renderer::ReadFinalImageData(void *buffer, GLenum format, GLenum type) {
    finalBuffer.Bind();
    glReadPixels(0, 0, width, height, format, type, buffer);
    finalBuffer.Unbind();
}

GLuint Renderer::GetFinalImageID() {
    return finalBuffer.GetTextureIDs()[0];
}
