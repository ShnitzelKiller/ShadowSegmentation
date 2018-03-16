//
// Created by James Noeckel on 3/13/18.
//

#ifndef RENDERER_BASICQUAD_H
#define RENDERER_BASICQUAD_H


#include "ScreenspaceQuad.h"

class TextureQuad : public ScreenspaceQuad {
public:
    void SetInvert(bool inv);
    void SetImage(GLuint tex);
private:
protected:
    void GetUniforms() override;
    void SetUniforms() override;
    void GetShader() override;
    GLint invertUniform;
    bool invert;
    GLuint textureID;
};


#endif //RENDERER_BASICQUAD_H
