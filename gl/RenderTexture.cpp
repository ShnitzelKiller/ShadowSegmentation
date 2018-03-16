//
// Created by James Noeckel on 3/4/18.
//

#include "RenderTexture.h"
#include <cstdio>
#include <iostream>

RenderTexture::RenderTexture(int width, int height, int num, GLuint internalformat) : count(num) {
    tex = new GLuint[num];
    GLenum attachments[num];
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glGenTextures(num, &tex[0]);
    for (int i=0; i<num; i++) {
        glBindTexture(GL_TEXTURE_2D, tex[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glBindTexture(GL_TEXTURE_2D, 0);
        attachments[i] = (GLenum) GL_COLOR_ATTACHMENT0 + i;
        glFramebufferTexture2D(GL_FRAMEBUFFER, attachments[i], GL_TEXTURE_2D, tex[i], 0);
    }
    glDrawBuffers(num, &attachments[0]);
    glGenRenderbuffers(1, &zbuf);
    glBindRenderbuffer(GL_RENDERBUFFER, zbuf);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, zbuf);
    GLuint status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        fprintf(stderr, "glCheckFramebufferStatus: %x\n", status);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    GLuint err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        fprintf(stderr, "error %x in framebuffer", err);
    }
}

RenderTexture::RenderTexture(RenderTexture &&other) noexcept : tex(other.tex), fbo(other.fbo), zbuf(other.zbuf), count(other.count) {
    other.tex = nullptr;
    other.fbo = 0;
    other.zbuf = 0;
}

RenderTexture::~RenderTexture() {
    glDeleteRenderbuffers(1, &zbuf);
    glDeleteFramebuffers(1, &fbo);
    if (tex) {
        glDeleteTextures(count, tex);
        delete[] tex;
    }
}

GLuint *RenderTexture::GetTextureIDs() {
    return tex;
}

int RenderTexture::Size() {
    return count;
}

void RenderTexture::Bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void RenderTexture::Unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderTexture::Clear(float r, float g, float b, float a) {
    Bind();
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Unbind();
}

GLuint RenderTexture::GetID() {
    return fbo;
}
