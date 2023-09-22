#include <iostream>
#include "Camara.h"
#include <cstdlib> // Include this for atoi()

void aleatoria() {
    vec3 center = vec3(0, 0, 0);
    vec3 up = vec3(0, 1, 0);

    Camara cam;
    cam.configurar(3, 60, 1000, 1000,
                   vec3(10, 30, 100),
                   center,
                   up);


    cam.renderizar(30);
}

void basica() {
    vector<Objeto *> objetos;
    Objeto *p1;
    p1 = new Esfera(vec3(10, 0, 0), 8, vec3(0, 0, 1));
    p1->init_constants(1, 0);
    p1->ke = 1;
    objetos.emplace_back(p1);

    p1 = new Esfera(vec3(20, 10, 20), 4, vec3(1,1,0));
    p1->init_constants(0.8, 0.2, 32);
    p1->light = true;
    //p1->ke = 0.9;
    //p1->transparency = true;
    //p1->ior = 1.5;

    objetos.emplace_back(p1);
    vector<Luz *> luces;
    Luz* pluz = new Luz( vec3(20, 10, 20), vec3(1,1,0));
    luces.emplace_back(pluz);

    p1 = new Esfera(vec3(0, 10, 0), 3, vec3(1, 1, 0));
    p1->init_constants(0.6, 0.4, 32);
    //p1->ke = 0.8;
    //p1->transparency = false;
    //p1->ior = 1.2;
    p1->light = true;
    objetos.emplace_back(p1);
    pluz =new Luz (vec3(0, 10, 0), vec3(1, 1, 0));
    luces.emplace_back(pluz);

    p1 = new Plano(vec3(0, 1, 0), 1, vec3(0.123, 0.456, 0.789));
    p1->init_constants(0.9, 0.1);
    p1->ke = 0.1;
    objetos.emplace_back(p1);

    p1 = new Cilindro(vec3(0,0,0), vec3(0,25,0), 7, vec3(1, 1, 1));
    p1->init_constants(0.7, 0.3, 8);
    p1->transparency = true;
//p1->ke  = 0.8;
    objetos.emplace_back(p1);




    Camara cam;
    for (int x = 0, n = 1; x <50; x+=3, n++) {
        cam.configurar(3, 100, 600, 800,
                       vec3(x, 5+x, 50),
                       vec3(0, 0, 0),
                       vec3(0, 1, 0));
        cam.renderizar(objetos, luces, n);
    }
}

void escena2() {
    vector<Objeto *> objetos;
    Objeto *p1;
    /*p1 = new Plano(vec3(0, 1, 0), 1, vec3(0.123, 0.456, 0.789));
    p1->init_constants(0.9, 0.1);
    p1->ke = 0.1;
    objetos.emplace_back(p1);
**/
    p1 = new Cilindro(vec3(-5, 4, 10), vec3(5, 14, 10), 5, vec3(0, 1, 1));
    p1->init_constants(0.7, 0.3, 8);
    p1->transparency = true;
    p1->ior = 1.3;
    objetos.emplace_back(p1);
/*
    p1 = new Esfera(vec3(5, 0, -20), 8, vec3(1, 0.1, 0.1));
    p1->init_constants(0.8, 0.2);
    p1->ke = 0.9;
    p1->transparency = true;
    p1->ior = 1.5;
    objetos.emplace_back(p1);
*/
    vector<Luz *> luces;
    Luz luz(vec3(30, 30, 30), vec3(1, 1, 1));
    luces.emplace_back(&luz);

    Camara cam;
    for (int y = 1, n = 1; y < 200; y += 4, n++) {
        cam.configurar(3, 60, 600, 800,
                       vec3(1, y, 50),
                       vec3(0, 0, 0),
                       vec3(0, 1, 0));
        //cam.renderizar(objetos, luces[0], n);
    }
}

void escena3() {
    vector<Objeto *> objetos;
    Objeto *p1;
    p1 = new Cilindro(vec3(50, -50, 210), vec3(100, 50, 210), 30, vec3(1, 1, 0));
    p1->kd = 0.9;
    p1->ks = 0.9;
    p1->transparency = true;
    p1->ior = 1;
    objetos.push_back(p1);

    p1 = new Esfera(vec3(180, 10, 200), 24, vec3(1, 0, 0));
    p1->kd = 0.9;
    p1->ks = 0.9;
    p1->transparency = false;
    p1->ior = 1;
    objetos.push_back(p1);

    p1 = new Esfera(vec3(80, 60, 200), 24, vec3(0, 0, 1));
    p1->kd = 0.9;
    p1->ks = 0.9;
    objetos.push_back(p1);

    p1 = new Triangulo(vec3(-200, 10, 300), vec3(800, 10, 300), vec3(100, 10, 0), vec3(0.529, 0.807, 0.921));
    p1->kd = 0.9;
    p1->ks = 0.9;
    p1->ke = 1;
    p1->transparency = false;
    p1->ior = 2;
    objetos.push_back(p1);


    vector<Luz *> luces;

    Luz luz(vec3(50, 1300, 50), vec3(1, 1, 1));
    luces.emplace_back(&luz);
    Camara cam;
    cam.configurar(3, 70, 800, 600,
                   vec3(150, 70, 350),
                   vec3(150, 70, 200),
                   vec3(0, 1, 0));
    //cam.renderizar(objetos, luces[0], 0);


}

void escena4() {
    vector<Luz *> luces;

    vec3 posLuz1(-10, 10, -10);
    vec3 colorLuz1(1, 1, 0);
    Esfera esfLuz(posLuz1, 2, colorLuz1);
    esfLuz.init_constants(0.8, 0.2, 32);
    esfLuz.light = true;
    Luz luz1(posLuz1, colorLuz1);
    luces.emplace_back(&luz1);

    Esfera esf(vec3(2, 0, 0), 8, vec3(0, 0, 1));
    esf.init_constants(0.8, 0.2, 32, 0.9);
    Esfera esf2(vec3(-10, 0, 10), 6, vec3(1, 0, 0));
    esf2.init_constants(0.8, 0.2, 32, 0, true, 1.5);
    Esfera esf3(vec3(-30, 0, -50), 10, vec3(1, 1, 0));
    esf3.init_constants(0.8, 0.2, 32, 0, true, 1.5);

    Plano plano(vec3(0, 1, 0), 10, vec3(0, 1, 1));
    plano.init_constants(0.3, 0.1, 8, 0.4);
    vector<Objeto *> objetos;
    objetos.emplace_back(&esf);
    //objetos.emplace_back(&esf2);
    //objetos.emplace_back(&esf3);
    //objetos.emplace_back(&esfLuz);
    objetos.emplace_back(&plano);
    Camara cam;
    cam.configurar(3, 60, 800, 600,
                   vec3(0, 10, 60),
                   vec3(0, 0, 0),
                   vec3(0, 1, 0));
    cam.renderizar(objetos, luces, 0);
}

int main() {
    //escena4();
    //escena3();
    //escena2();
    basica();
    return 0;
}
