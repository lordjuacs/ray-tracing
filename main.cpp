#include <cstdlib> // Include this for atoi()
#include <iostream>
#include <random>
#include "Camara.h"
#include <chrono>

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
    vector < Objeto * > objetos;
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

    vector < Luz * > luces;
    Luz luz(vec3(30, 30, 30), vec3(1, 1, 1));
    luces.emplace_back(&luz);
    p1 = new Esfera(vec3(30, 30, 30), 3, vec3(1, 1, 1));
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
    vector < Objeto * > objetos;
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


    vector < Luz * > luces;

    Luz luz(vec3(50, 1300, 50), vec3(1, 1, 1));
    luces.emplace_back(&luz);
    Camara cam;
    cam.configurar(3, 120, 800, 600,
                   vec3(150, 70, 350),
                   vec3(150, 70, 200),
                   vec3(0, 1, 0));
    cam.renderizar(objetos, luces, -1);


}





void basica() {
    //Plano, transparente, color morado
    Objeto *plano = new Plano(vec3(0, 1, 0), 1, vec3(0.396, 0.255, 0.522));
    plano->init_constants(0.9, 0.1, 12, 0.3);

    //Botella
    Objeto *cil1 = new Cilindro(vec3(0, 0, 0), vec3(0, 90, 0), 80, vec3(1, 1, 1));
    cil1->init_constants(0.7, 0.3, 20, 0.8, true, 2);
    Objeto *cil2 = new Cilindro(vec3(0, 90, 0), vec3(0, 110, 0), 30, vec3(0.5, 0.5, 0.5));
    cil2->init_constants(0.7, 0.3, 20);
    Objeto *cil3 = new Cilindro(vec3(-150, 20, -100), vec3(-170, 50, -140), 40, vec3(0.94, 0.14, 0.8));
    cil3->init_constants(0.7, 0.3, 20);

    //Esferas
    Objeto *esf1 = new Esfera(vec3(0, 30, 150), 15, vec3(1, 1, 0)); // amarillo +z
    esf1->init_constants(0.9, 0.9, 10, 0.6);
    Objeto *esf2 = new Esfera(vec3(0, 40, -150), 15, vec3(0, 0, 1)); // azul -z
    esf2->init_constants(0.9, 0.9, 10, 0.6);
    Objeto *esf3 = new Esfera(vec3(150, 50, 0), 15, vec3(1, 0, 0)); // rojo +x
    esf3->init_constants(0.9, 0.9, 10, 0.6);
    Objeto *esf4 = new Esfera(vec3(-150, 60, 0), 15, vec3(0, 1, 0)); // verde -x
    esf4->init_constants(0.9, 0.9, 10, 0.6);

    // Triangulos
    Objeto *tri1 = new Triangulo(vec3(70,30,100),vec3(80,90,150),vec3(50,100,200),vec3(0.5, 0.7, 0.9));
    tri1->init_constants(0.9, 0.9, 10, 0.6);
    Objeto *tri2 = new Triangulo(vec3(-150,50,-90),vec3(-50,200,-10),vec3(-200,180,-50),vec3(0.97,0.62,0.06));
    tri2->init_constants(0.9, 0.9, 30, 0.8);

    vector < Objeto * > objetos;
    objetos.emplace_back(plano);
    objetos.emplace_back(cil1);
    objetos.emplace_back(cil2);
    objetos.emplace_back(cil3);
    objetos.emplace_back(esf1);
    objetos.emplace_back(esf2);
    objetos.emplace_back(esf3);
    objetos.emplace_back(esf4);
    objetos.emplace_back(tri1);
    objetos.emplace_back(tri2);

    //Esferas de luz
    vector < Luz * > luces;
    vec3 posLuz1 = vec3(0, 30, 0), colLuz1(0.9, 0.9, 0);
    Objeto *esfLuz1 = new Esfera(posLuz1, 5, colLuz1);
    esfLuz1->light = true;
    objetos.emplace_back(esfLuz1);
    Luz *luz1 = new Luz(posLuz1, colLuz1);
    Luz *luzExterna = new Luz(vec3(2000, 2000, 2000), vec3(1, 1, 1));
    //luces.emplace_back(luzExterna);
    luces.emplace_back(luz1);

    Camara cam;
    const int numFrames = 20;

    float posX = 150;
    float posY = 120;
    float posZ = 50;
    vec3 center = vec3(0, 0, 0);
    vec3 eye = vec3(posX, posY, posZ);
    int posYDirection = 1; // 1 for up, -1 for down
    float totalYDistance = 20;
    float posYIncrement = 1.0f; // Change in posY per iteration
    auto start = std::chrono::high_resolution_clock::now();

    for (int frame = 0; frame < numFrames; ++frame) {
        // Calculate the camera's position on a circular path
        float angle = 2 * M_PI * frame / numFrames; // Angle in radians
        float radius = sqrt(pow(posX, 2) + pow(posZ, 2)); // Calculate the distance from (0,0,0)

        // Update the eye position to follow a circular path around (0,0,0)
        posX = radius * cos(angle);
        posZ = radius * sin(angle);

        // Update posY based on direction (up or down)
        posY += posYDirection * posYIncrement;

        // Switch direction when reaching the top or bottom of the interval
        if (posYDirection == 1 && posY >= 140) {
            posYDirection = -1; // Switch to moving down
        } else if (posYDirection == -1 && posY <= 120) {
            posYDirection = 1; // Switch to moving up
        }

        cam.configurar(3, 140, 600, 800,
                       vec3(posX, posY, posZ), // Update eye position
                       center,
                       vec3(0, 1, 0));

        cam.renderizar(objetos, luces, frame + 1);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << "Time taken by loop: " << duration.count() << " seconds" << std::endl;


    

}

int main() {
    //escena5();
    //escena4();
    //escena3();
    //escena2();
    basica();
    return 0;
}
