//
// Created by James Noeckel on 3/13/18.
//

#ifndef RENDERER_DILATEQUAD_H
#define RENDERER_DILATEQUAD_H


#include "ScreenspaceQuad.h"
#include "gl/RenderTexture.h"

class DilateQuad : public ScreenspaceQuad {
public:
    DilateQuad(int width, int height, int radius=1);
    void Render() override;
    void SetRadius(int rad);
private:
    int width, height;
    int radius;
    RenderTexture rt;
protected:
    void SetUniforms() override;
    void GetShader() override;
    GLint dimUniform;
    GLint radUniform;
    GLint widthUniform;
    GLint heightUniform;
};


#endif //RENDERER_DILATEQUAD_H
