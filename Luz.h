//
// Created by juacs on 07/09/2023.
//

#ifndef RAYTRACING_LUZ_H
#define RAYTRACING_LUZ_H

#include "vec3.h"

class Luz {
public:
    vec3 pos, color;

    Luz(vec3 _pos, vec3 _color) {
        pos = _pos;
        color = _color;
    }
};


#endif //RAYTRACING_LUZ_H
