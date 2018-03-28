//
// Created by James Noeckel on 3/4/18.
//

#ifndef RENDERER_RENDERTEXTURE_H
#define RENDERER_RENDERTEXTURE_H

#include <GL/glew.h>


class RenderTexture {
public:
    /**
     *
     * @param width
     * @param height
     * @param num Number of color textures (for multiple render buffers)
     * @param internalformat
     */
    RenderTexture(int width, int height, int num = 1, GLuint internalformat = GL_RGBA);
    RenderTexture(RenderTexture &&other) noexcept; //no copying allowed
    ~RenderTexture();
    /**
     * Number of render buffers
     */
    int Size();
    /**
     * Get pointer to array of render textures
     */
    GLuint *GetTextureIDs();
    /**
     * Bind this framebuffer object
     */
    void Bind();
    /**
     * Revert to default framebuffer
     */
    void Unbind();
    /**
     * Clear this framebuffer with the specified color
     * @param r
     * @param g
     * @param b
     * @param a
     */
    void Clear(float r, float g, float b, float a);
    GLuint GetID();
private:
    GLuint fbo;
    GLuint *tex;
    GLuint zbuf;
    int count;
};


#endif //RENDERER_RENDERTEXTURE_H
