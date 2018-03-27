//
// Created by James Noeckel on 2/20/18.
//

#ifndef RENDERER_RENDERER_H
#define RENDERER_RENDERER_H


#include "Scene.h"
#include "../gl/shaders.h"
#include "../gl/RenderTexture.h"
#include "../overlays/TextureQuad.h"

class Renderer {
public:
    Renderer(Scene *scene, int width, int height, GLenum internalformat = GL_RGBA);
    void Render(bool shading = true);
    ~Renderer();
    std::vector<GLuint> GetImages();
    GLuint GetFinalImageID();
    int GetWidth() const;
    int GetHeight() const;
    void ReadImageData(void *buffer, GLenum format, GLenum type, int index);
    void ReadFinalImageData(void *buffer, GLenum format, GLenum type);
private:
    void Update();
    Scene *scene;
    std::vector<RenderTexture> rendertextures;
    RenderTexture finalBuffer;
    TextureQuad quad;
    std::shared_ptr<Program> program;
    GLint mvp_uniform;
    GLint nmw_uniform;
    int width;
    int height;
    int num_buffers;
    GLenum internalformat;
};


#endif //RENDERER_RENDERER_H
