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
    float ke; // constante material espejo/reflexion [0,1>, nunca se refleja el 100%
    bool transparency; // objeto es transparente o no
    float ior; // index of refraction
    bool light; // objeto es luz o no
    Objeto(vec3 col, float kd = 1) : color{col}, kd{kd} {
        init_constants(kd);
    }

    void
    init_constants(float kd_ = 1, float ks_ = 1, float n_ = 8, float ke_ = 0, bool transparency_ = false,
                   float ior_ = 1, bool light_ = false) {
        this->kd = kd_;
        this->ks = ks_;
        this->n = n_;
        this->ke = ke_;
        this->transparency = transparency_;
        this->ior = ior_;
        this->light = light_;

    }

    virtual bool intersectar(Rayo ray, float &t, vec3 &normal) = 0;
};

class Esfera : public Objeto {
public:
    vec3 centro;
    float radio;

    Esfera(vec3 cen, float r, vec3 col, float kd = 1) : centro{cen}, radio{r}, Objeto(col, kd) {}

    bool intersectar(Rayo ray, float &t, vec3 &normal) override {
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

    Plano(vec3 normal, float d_, vec3 col) : pnormal(normal), d(d_), Objeto(col) {
        pnormal.normalize();
    }

    bool intersectar(Rayo ray, float &t, vec3 &normal) override {
        float den = ray.dir.punto(pnormal);
        if (den != 0) {
            vec3 p1 = pnormal * d; // punto en el plano
            t = (p1 - ray.ori).punto(pnormal) / den;
            if (t < 0) return false; // or just t < 0????
            normal = pnormal; //
            return true;

        }
        return false;
    }
};

class Triangulo : public Objeto {
public:
    vec3 p1;
    vec3 p2;
    vec3 p3;

    Triangulo(vec3 v1, vec3 v2, vec3 v3, vec3 col, float kd = 1) : p1(v1), p2(v2), p3(v3),
                                                                   Objeto(col, kd) {}

    bool intersectar(Rayo ray, float &t, vec3 &normal) override {
        vec3 n = (p2 - p1).cruz(p3 - p2);
        n.normalize();
        float den = ray.dir.punto(n);
        if (den != 0) {
            t = (p1 - ray.ori).punto(n) / den;
            if (t <= 0) return false; // or just t < 0????
            normal = n;
            //revisar si punto esta dentro del triangulo
            vec3 pint = ray.ori + t * ray.dir;
            if (n.punto((p2 - p1).cruz(pint - p1)) > 0 && n.punto((p3 - p2).cruz(pint - p2)) > 0 &&
                n.punto((p1 - p3).cruz(pint - p3)) > 0)
                return true;

        }
        return false;
    }
};

class Cilindro : public Objeto {
public:
    vec3 pa, pb;
    float radio;

    Cilindro(vec3 pa_, vec3 pb_, float rad, vec3 col, float kd = 1) : pa(pa_), pb(pb_), radio(rad), Objeto(col, kd) {}

    bool intersectar(Rayo ray, float &t, vec3 &normal) override {
        vec3 ro = ray.ori;
        vec3 rd = ray.dir;
        vec3 ba = pb - pa;
        vec3 oc = ro - pa;

        float baba = ba.punto(ba);
        float bard = ba.punto(rd);
        float baoc = ba.punto(oc);

        float k2 = baba - bard * bard;
        float k1 = baba * oc.punto(rd) - baoc * bard;
        float k0 = baba * oc.punto(oc) - baoc * baoc - radio * radio * baba;

        float h = k1 * k1 - k2 * k0;
        if (h < 0.0) return false;
        //body
        h = sqrt(h);
        t = (-k1 - h) / k2;
        if (t <= 0) return false;
        float y = baoc + t * bard;
        if (y > 0.0 && y < baba && t > 0) {
            normal = (oc + t * rd - ba * y / baba) / radio;
            normal.normalize();
             return true;
        }
        // caps
        t = (((y < 0.0) ? 0.0 : baba) - baoc) / bard;
        if (abs(k1 + k2 * t) < h) {
            normal = ba * sgn(y) / baba;
            normal.normalize();
            return true;
        }
        return false;

    }
};

#endif //CG2023_OBJETO_H
