//
// Created by James Noeckel on 2/20/18.
//

#ifndef RENDERER_RENDERER_H
#define RENDERER_RENDERER_H


#include "Scene.h"
#include "../gl/shaders.h"
#include "../gl/RenderTexture.h"

class Renderer {
public:
    Renderer(Scene *scene, int width, int height);
    void Render();
    ~Renderer();
    std::vector<GLuint> GetImages();
private:
    void Update();
    Scene *scene;
    std::vector<RenderTexture> rendertextures;
    Program *program;
    GLint mvp_uniform;
    GLint nmw_uniform;
    int width;
    int height;
    int num_buffers;
};


#endif //RENDERER_RENDERER_H
