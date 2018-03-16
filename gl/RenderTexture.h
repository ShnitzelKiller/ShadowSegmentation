//
// Created by James Noeckel on 3/4/18.
//

#ifndef RENDERER_RENDERTEXTURE_H
#define RENDERER_RENDERTEXTURE_H

#include <GL/glew.h>


class RenderTexture {
public:
    RenderTexture(int width, int height, int num = 1, GLuint internalformat = GL_RGBA);
    RenderTexture(RenderTexture &&other) noexcept; //no copying allowed
    ~RenderTexture();
    int Size();
    GLuint *GetTextureIDs();
    void Bind();
    void Unbind();
    void Clear(float r, float g, float b, float a);
    GLuint GetID();
private:
    GLuint fbo;
    GLuint *tex;
    GLuint zbuf;
    int count;
};


#endif //RENDERER_RENDERTEXTURE_H
