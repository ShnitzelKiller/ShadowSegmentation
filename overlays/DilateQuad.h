//
// Created by James Noeckel on 3/13/18.
//

#ifndef RENDERER_DILATEQUAD_H
#define RENDERER_DILATEQUAD_H


#include "ScreenspaceQuad.h"
#include "../gl/RenderTexture.h"
#include "TextureQuad.h"

class DilateQuad : public TextureQuad {
public:
    DilateQuad(int width, int height, int radius=1);
    void Render() override;
    void SetRadius(int rad);
private:
    RenderTexture rt;
protected:
    void SetUniforms() override;
    void GetUniforms() override;
    void GetShader() override;
    GLint dimUniform;
    GLint radUniform;
    GLint widthUniform;
    GLint heightUniform;
    int width, height;
    int radius;
};


#endif //RENDERER_DILATEQUAD_H
