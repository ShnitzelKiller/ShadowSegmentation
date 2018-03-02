//
// Created by James Noeckel on 2/20/18.
//

#ifndef RENDERER_RENDERER_H
#define RENDERER_RENDERER_H


#include "Scene.h"
#include "../shaders.h"

class Renderer {
public:
    Renderer(Scene *scene, int width, int height);
    void Render();
    ~Renderer();
    GLuint *GetImages(size_t *length);
private:
    Scene *scene;
    GLuint *framebuffers;
    GLuint *images;
    Program *program;
    GLint mvp_uniform;
    GLint nmw_uniform;
    int width;
    int height;
    int num_buffers;
};


#endif //RENDERER_RENDERER_H
