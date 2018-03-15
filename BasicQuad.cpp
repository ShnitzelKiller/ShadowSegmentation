//
// Created by James Noeckel on 3/13/18.
//

#include <iostream>
#include "BasicQuad.h"



void BasicQuad::SetUniforms() {
    ScreenspaceQuad::SetUniforms();
    invertUniform = program->GetUniformLocation("invert");
    program->Use();
    glUniform1f(invertUniform, 0);
    program->Unuse();

}

void BasicQuad::SetInvert(bool inv) {
//    std::cout << "setting invert (uniform " << invertUniform << ") to " << inv << std::endl;
    invert = inv;
    program->Use();
    glUniform1f(invertUniform, inv ? 1.0f : 0.0f);
    program->Unuse();
}