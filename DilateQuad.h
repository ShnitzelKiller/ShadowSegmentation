//
// Created by James Noeckel on 3/13/18.
//

#ifndef RENDERER_DILATEQUAD_H
#define RENDERER_DILATEQUAD_H


#include "ScreenspaceQuad.h"

class DilateQuad : public ScreenspaceQuad {
public:
    void SetRadius(int rad);
    void Render() override;
private:
protected:
    void SetUniforms() override;
    void GetShader() override;
    GLint dimUniform;
    GLint radUniform;
    GLint widthUniform;
    GLint heightUniform;
};


#endif //RENDERER_DILATEQUAD_H
