#include <cstdlib> // Include this for atoi()
#include <iostream>
#include <random>
#include "Camara.h"

void aleatoria() {
    vec3 center = vec3(0, 0, 0);
    vec3 up = vec3(0, 1, 0);

    Camara cam;
    cam.configurar(3, 60, 1000, 1000,
                   vec3(10, 30, 100),
                   center,
                   up);


}

void escena2() {
    vector<Objeto *> objetos;
    Objeto *p1;
    p1 = new Plano(vec3(0, 1, 0), 1, vec3(0.123, 0.456, 0.789));
    p1->init_constants(0.9, 0.1);
    p1->ke = 0.1;
    objetos.emplace_back(p1);

    p1 = new Cilindro(vec3(-5, 4, 10), vec3(5, 14, 10), 5, vec3(0, 1, 1));
    p1->init_constants(0.7, 0.3, 8);
    p1->transparency = true;
    p1->ior = 1.3;
    objetos.emplace_back(p1);

    p1 = new Esfera(vec3(5, 0, -20), 8, vec3(1, 0.1, 0.1));
    p1->init_constants(0.8, 0.2);
    p1->ke = 0.9;
    p1->transparency = true;
    p1->ior = 1.5;
    objetos.emplace_back(p1);

    vector<Luz *> luces;
    Luz luz(vec3(30, 30, 30), vec3(1, 1, 1));
    luces.emplace_back(&luz);
    p1 = new Esfera(vec3(30,30,30), 3,vec3(1,1,1));
    p1->light = true;
    objetos.emplace_back(p1);

    Camara cam;
    for (int y = 1, n = 1; y < 200; y += 4, n++) {
        cam.configurar(3, 60, 600, 800,
                       vec3(1 + y, y, 50),
                       vec3(0, 0, 0),
                       vec3(0, 1, 0));
        cam.renderizar(objetos, luces, n);
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
    cam.configurar(3, 120, 800, 600,
                   vec3(150, 70, 350),
                   vec3(150, 70, 200),
                   vec3(0, 1, 0));
    cam.renderizar(objetos, luces, -1);


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


void escena5() {
    vector<Objeto *> objetos;
    Objeto *cil = new Cilindro(vec3(150, 10, 210), vec3(150, 100, 210), 50, vec3(1, 1, 1));
    cil->init_constants(0.9, 0.9, 8, false, true, 1, false);
    //objetos.push_back(cil);
    auto *cil2 = new Cilindro(vec3(150, 100, 210), vec3(150, 120, 210), 25, vec3(1, 0, 0));
    cil2->init_constants(0.9, 0.9);
    auto *cil3 = new Cilindro(vec3(260, 40, 150), vec3(290, 60, 150), 10, vec3(1, 1, 0));
    cil3->init_constants(0.9, 0.9, 8, false, false, 1, false);
    objetos.push_back(cil2);
    objetos.push_back(cil3);
    auto *tri = new Triangulo(vec3(-200, 10, 300), vec3(800, 10, 300), vec3(100, 10, 0), vec3(0.529, 0.807, 0.921));
    tri->init_constants(0.9, 0.9, 8, 1, false, 2, false);
    objetos.push_back(tri);
    std::vector<Luz *> luces;
    Luz luz(vec3(150, 600, 210), vec3(1, 1, 1));
    luces.push_back(&luz);
    auto *esf = new Esfera(vec3(280, 80, 150), 12, vec3(1, 0, 0));
    esf->init_constants(0.9, 0.9, 8, 0, false, 0, false);
    auto *esf2 = new Esfera(vec3(0, 80, 150), 12, vec3(0, 1, 0));
    esf2->init_constants(0.9, 0.9, 8, 0, false, 0, false);
    auto *esf3 = new Esfera(vec3(40, 10, 150), 22, vec3(0, 0, 1));
    esf3->init_constants(0.9, 0.9, 8, 0, false, 0, false);
    auto *esf4 = new Esfera(vec3(150, 150, 210), 12, vec3(0, 1, 0));
    esf4->init_constants(0.9, 0.9, 8, 0, false, 0, false);
    objetos.push_back(esf);
    objetos.push_back(esf2);
    objetos.push_back(esf3);
    //objetos.push_back(esf4);
    Camara cam;
    cam.configurar(3, 60, 800, 600,
                   vec3(150, 70, 480),
                   vec3(150, 70, 200),
                   vec3(0, 1, 0));
    cam.renderizar(objetos, luces, -1);

}

void basica() {
    //Plano, transparente, color morado
    Objeto *plano = new Plano(vec3(0, 1, 0), 1, vec3(0.396, 0.255, 0.522));
    plano->init_constants(0.9, 0.1, 12, 0.3);

    //Botella
    Objeto *cil1 = new Cilindro(vec3(0, -50, 0), vec3(0, 50, 0), 30, vec3(1, 1, 1));
    cil1->init_constants(0.7, 0.3, 20, 0.8, true, 2);

    //Esferas
    Objeto *esf1 = new Esfera(vec3(0, 10, 50), 15, vec3(1, 1, 0)); // amarillo +z
    esf1->init_constants(0.9, 0.9, 10, 0.6);
    Objeto *esf2 = new Esfera(vec3(0, 20, -50), 15, vec3(0, 0, 1)); // azul -z
    esf2->init_constants(0.9, 0.9, 10, 0.6);
    Objeto *esf3 = new Esfera(vec3(50, 30, 0), 15, vec3(1, 0, 0)); // rojo +x
    esf3->init_constants(0.9, 0.9, 10, 0.6);
    Objeto *esf4 = new Esfera(vec3(-50, 40, 0), 15, vec3(0, 1, 0)); // verde -x
    esf4->init_constants(0.9, 0.9, 10, 0.6);

    vector<Objeto *> objetos;
    objetos.emplace_back(plano);
    objetos.emplace_back(cil1);
    objetos.emplace_back(esf1);
    objetos.emplace_back(esf2);
    objetos.emplace_back(esf3);
    objetos.emplace_back(esf4);

    //Esferas de luz
    vector<Luz *> luces;
    vec3 posLuz1 = vec3(0, 20, 0), colLuz1(1, 1, 1);
    Objeto *esfLuz1 = new Esfera(posLuz1, 10, colLuz1);
    esfLuz1->light = true;
    objetos.emplace_back(esfLuz1);
    Luz *luz1 = new Luz(posLuz1, colLuz1);
    Luz *luzExterna = new Luz(vec3(10, 200, 200), vec3(1, 1, 1));
    luces.emplace_back(luzExterna);
    luces.emplace_back(luz1);

    Camara cam;
    const int numFrames = 50;
    const float radius = 50.0f; // Radius of the circular path
    const float centerX = 20.0f; // Center X-coordinate
    const float centerY = 20.0f; // Center Y-coordinate

    for (int frame = 0; frame < numFrames; ++frame) {
        // Calculate the camera's position on a circular path
        float angle = 2 * M_PI * frame / numFrames; // Angle in radians
        float x = centerX + radius * cos(angle);
        float y = centerY + radius * sin(angle);
        cout << "xd";
        cam.configurar(3, 150, 600, 800,
                       vec3(x, 50, y),
                       vec3(0, 0, 0),
                       vec3(0, 1, 0));
        cam.renderizar(objetos, luces, frame + 1);
    }



}

int main() {
    //escena5();
    //escena4();
    //escena3();
    //escena2();
    basica();
    return 0;
}
