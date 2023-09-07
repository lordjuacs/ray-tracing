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
    float kd; //constante difusa del material [0,1]
    float ks; //constante especular del material [0,1]
    float n; //shininess exponent
    Objeto(vec3 col, float kd = 1) : color{col}, kd{kd} {}

    void setConstantes(float kd = 1, float ks = 1, float n = 8) {
        this->kd = kd;
        this->ks = ks;
        this->n = n;
    }

    virtual bool intersectar(Rayo ray, float &t, vec3 &normal) = 0;
};

class Esfera : public Objeto {
public:
    vec3 centro;
    float radio;

    Esfera(vec3 cen, float r, vec3 col, float kd = 1) : centro{cen}, radio{r}, Objeto(col, kd) {}

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
        vec3 pi = ray.ori + t * ray.dir;
        normal = pi - centro;
        normal.normalize();
        return true;
    }
};

class Plano : public Objeto {
public:
    vec3 pnormal;
    float d; //distancia al origen de coordenadas
    float radio;

    Plano(vec3 normal, float d_, vec3 col) : pnormal(normal), d(d_), Objeto(col) {
        pnormal.normalize();
    }

    bool intersectar(Rayo ray, float &t, vec3 &normal) {
        float den = ray.dir.punto(pnormal);
        if (den != 0) {
            vec3 p1 = pnormal * d; // punto en el plano
            t = (p1 - ray.ori).punto(pnormal) / den;
            if (t <= 0) return false; // or just t < 0
            normal = pnormal;
            return true;

        }
        return false;
    }
};

#endif //CG2023_OBJETO_H
