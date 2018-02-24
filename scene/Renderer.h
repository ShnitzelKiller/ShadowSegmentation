//
// Created by James Noeckel on 2/20/18.
//

#ifndef RENDERER_RENDERER_H
#define RENDERER_RENDERER_H


#include "Scene.h"

class Renderer {
public:
    void Init(Scene *scene, int width, int height);
    void Render();
    void Destroy();
    GLuint *GetImages(size_t *length);
private:
    Scene *scene;
    GLuint *framebuffers;
    GLuint *images;
    GLuint shaderProgram;
    GLint mvp_uniform;
    int width;
    int height;
    int num_buffers;
};


#endif //RENDERER_RENDERER_H
