//
// Created by James Noeckel on 3/13/18.
//

#ifndef RENDERER_BASICQUAD_H
#define RENDERER_BASICQUAD_H


#include "ScreenspaceQuad.h"

class BasicQuad : public ScreenspaceQuad {
public:
    void SetInvert(bool inv);
private:
protected:
    void SetUniforms() override;
    GLint invertUniform;
    bool invert;
};


#endif //RENDERER_BASICQUAD_H
