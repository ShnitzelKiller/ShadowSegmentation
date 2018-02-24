//
// Created by James Noeckel on 2/23/18.
//

#include "mathdebug.h"
#include <iostream>

void printmat4(glm::mat4 &VP) {
    std::cout << VP[0][0] << ' ' << VP[1][0] << ' ' << VP[2][0] << ' ' << VP[3][0] << std::endl
              << VP[0][1] << ' ' << VP[1][1] << ' ' << VP[2][1] << ' ' << VP[3][1] << std::endl
              << VP[0][2] << ' ' << VP[1][2] << ' ' << VP[2][2] << ' ' << VP[3][2] << std::endl
              << VP[0][3] << ' ' << VP[1][3] << ' ' << VP[2][3] << ' ' << VP[3][3] << std::endl;
}