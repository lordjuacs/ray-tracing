//
// Created by hgallegos on 30/03/2023.
//

#ifndef CG2023_OBJETO_H
#define CG2023_OBJETO_H

#include "vec3.h"
#include "Rayo.h"

class Objeto {
public:
    vec3 color;
    float kd = 1; //constante difusa del material [0,1]
    float ks = 1; //constante especular del material [0,1]
    float n; //shininess exponent
    Objeto(vec3 col) : color{col} {}

    virtual bool intersectar(Rayo ray, float &t, vec3 &normal) = 0;
};

class Esfera : public Objeto {
public:
    vec3 centro;
    float radio;

    Esfera(vec3 cen, float r, vec3 col) :
            centro{cen}, radio{r}, Objeto(col) {}

    bool intersectar(Rayo ray, float &t, vec3 &normal) {
        vec3 d = ray.dir;
        vec3 o = ray.ori;
        float a = d.punto(d);
        float b = 2 * d.punto(o - centro);
        float c = (o - centro).punto(o - centro) - radio * radio;
        float D = b * b - 4 * a * c;
        if (D <= 0) { return false; }
        float t1 = (-b + sqrt(D)) / (2 * a);
        float t2 = (-b - sqrt(D)) / (2 * a);
        t = std::min(t1, t2);
        if (t <= 0) { return false; }
        vec3 pi = ray.ori - t * ray.dir;
        normal = pi - centro;
        normal.normalize();
        return true;

    }
};


#endif //CG2023_OBJETO_H
