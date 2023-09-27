#include <cstdlib> // Include this for atoi()
#include <iostream>
#include <random>
#include "Camara.h"
#include <chrono>


struct Firefly {
    Objeto *esfera;
    Luz *luz;
    float fx;
    float fz;
    float px;
    float pz;
    float R;
    float circle_center_x;
    float circle_center_z;
};

void basica() {
    //Plano, transparente, color morado
    Objeto *plano = new Plano(vec3(0, 1, 0), 1, vec3(0.396, 0.255, 0.522));
    plano->init_constants(0.9, 0.1, 12, 0.3);

    //Botella
    float radio_botella = 80;
    Objeto *cil1 = new Cilindro(vec3(0, 0, 0), vec3(0, 90, 0), radio_botella, vec3(1, 1, 1));
    cil1->init_constants(0.7, 0.3, 20, 0.8, true, 2);
    Objeto *cil2 = new Cilindro(vec3(0, 90, 0), vec3(0, 110, 0), 30, vec3(0.5, 0.5, 0.5));
    cil2->init_constants(0.7, 0.3, 20);
    Objeto *cil3 = new Cilindro(vec3(-150, 20, -100), vec3(-170, 50, -140), 40, vec3(0.94, 0.14, 0.8));
    cil3->init_constants(0.7, 0.3, 20);
    Objeto *cil4 = new Cilindro(vec3(-100, 50, 200), vec3(-120, 120, 150), 50, vec3(0.66, 0.35, 0.54));
    cil4->init_constants(0.8, 0.9, 8);

    //Esferas
    Objeto *esf1 = new Esfera(vec3(0, 30, 150), 15, vec3(0.78, 0.39,0.9)); // lila +z
    esf1->init_constants(0.9, 0.9, 10, 0.6);
    Objeto *esf2 = new Esfera(vec3(0, 40, -150), 15, vec3(0, 0, 1)); // azul -z
    esf2->init_constants(0.9, 0.9, 10, 0.6);
    Objeto *esf3 = new Esfera(vec3(150, 50, 0), 15, vec3(1, 0, 0)); // rojo +x
    esf3->init_constants(0.9, 0.9, 10, 0.6);
    Objeto *esf4 = new Esfera(vec3(-150, 60, 0), 15, vec3(0, 1, 0)); // verde -x
    esf4->init_constants(0.9, 0.9, 10, 0.6);

    // Triangulos
    Objeto *tri1 = new Triangulo(vec3(70, 30, 100), vec3(80, 90, 150), vec3(50, 100, 200), vec3(0.5, 0.7, 0.9));
    tri1->init_constants(0.9, 0.9, 10, 0.6);
    Objeto *tri2 = new Triangulo(vec3(-150, 50, -90), vec3(-50, 200, -10), vec3(-200, 180, -50),
                                 vec3(0.97, 0.62, 0.06));
    tri2->init_constants(0.9, 0.9, 30, 0.8);

    vector<Objeto *> objetos;
    objetos.emplace_back(plano);
    objetos.emplace_back(cil1);
    objetos.emplace_back(cil2);
    objetos.emplace_back(cil3);
    objetos.emplace_back(cil4);
    objetos.emplace_back(esf1);
    objetos.emplace_back(esf2);
    objetos.emplace_back(esf3);
    objetos.emplace_back(esf4);
    objetos.emplace_back(tri1);
    objetos.emplace_back(tri2);

    //Esferas de luz
    vector<Luz *> luces;
    vec3 colLuz(0.9, 0.9, 0);

    std::random_device rd;
    std::mt19937 gen(rd());
    // Define the parameter ranges
    float min_fx = 0.01;  // Reduce the range
    float max_fx = 0.05;  // Reduce the range
    float min_fz = 0.01;  // Reduce the range
    float max_fz = 0.05;  // Reduce the range
    float min_px = 0.0;
    float max_px = 2 * M_PI;
    float min_pz = 0.0;
    float max_pz = 2 * M_PI;
    float min_R = 30;
    float max_R = radio_botella - 20 - 10;
    float min_circle_center_x = 5;
    float max_circle_center_x = 20;
    float min_circle_center_z = 5;
    float max_circle_center_z = 20;

    // Create distributions for each parameter
    std::uniform_real_distribution<float> circle_center_x_dist(min_circle_center_x, max_circle_center_x);
    std::uniform_real_distribution<float> circle_center_z_dist(min_circle_center_z, max_circle_center_z);
    std::uniform_real_distribution<float> R_dist(min_R, max_R);
    std::uniform_real_distribution<float> fx_dist(min_fx, max_fx);
    std::uniform_real_distribution<float> fz_dist(min_fz, max_fz);
    std::uniform_real_distribution<float> px_dist(min_px, max_px);
    std::uniform_real_distribution<float> pz_dist(min_pz, max_pz);

    Objeto *firefly;
    Luz *fireflyLight;
    vector<Firefly> luciernagas;
    int total_fireflies = 15;
    float fx;
    float fz;
    float px;
    float pz;
    float R;
    float circle_center_x;
    float circle_center_z;
    for (int i = 0, vertical = 10; i < total_fireflies; vertical += 5, ++i) {
        firefly = new Esfera(vec3(0, vertical, 0), 3, colLuz);
        firefly->light = true;
        fireflyLight = new Luz(vec3(0, vertical, 0), colLuz);
        // Generate random values for each parameter
        fx = fx_dist(gen);
        fz = fz_dist(gen);
        px = px_dist(gen);
        pz = pz_dist(gen);
        R = R_dist(gen);
        circle_center_x = circle_center_x_dist(gen);
        circle_center_z = circle_center_z_dist(gen);

        Firefly luciernaga(firefly, fireflyLight, fx, fz, px, pz, R, circle_center_x, circle_center_z);
        objetos.emplace_back(luciernaga.esfera);
        luces.emplace_back(luciernaga.luz);
        luciernagas.emplace_back(luciernaga);
    }

    const int numFrames = 50;


    Camara cam;

    float posX = 150;
    float posY = 100;
    float posZ = 50;
    vec3 center = vec3(0, 0, 0);
    int posYDirection = 1; // 1 for up, -1 for down
    float posYIncrement = 1.0f; // Change in posY per iteration
    auto start = std::chrono::high_resolution_clock::now();

    float rotationSpeed = 0.09;
    for (int frame = 0; frame < numFrames; ++frame) {

        for (auto luciernaga: luciernagas) {
            auto esfera = dynamic_cast<Esfera *>(luciernaga.esfera);
            if (esfera) {
                esfera->centro.x = luciernaga.circle_center_x + luciernaga.R * cos(2 * M_PI * luciernaga.fx * frame + luciernaga.px);
                esfera->centro.z = luciernaga.circle_center_z + luciernaga.R * sin(2 * M_PI * luciernaga.fz * frame + luciernaga.pz);
                float dist_center = sqrt(pow(esfera->centro.x - luciernaga.circle_center_x, 2) + pow(esfera->centro.z - luciernaga.circle_center_z, 2));
                if (dist_center > luciernaga.R || dist_center > radio_botella) {
                    //# If a sphere goes outside the circular region, move it back to the boundary
                    float theta = atan2(esfera->centro.z - luciernaga.circle_center_z, esfera->centro.x - luciernaga.circle_center_x);
                    esfera->centro.x = luciernaga.circle_center_x + luciernaga.R * cos(theta);
                    esfera->centro.z = luciernaga.circle_center_z + luciernaga.R * sin(theta);
                }
                luciernaga.luz->pos.x = esfera->centro.x;
                luciernaga.luz->pos.z = esfera->centro.z;
            }

        }


        // Calculate the camera's position on a circular path
        float angle = rotationSpeed * 2 * M_PI * frame / numFrames; // Angle in radians
        float radius = sqrt(pow(posX, 2) + pow(posZ, 2)); // Calculate the distance from (0,0,0)

        // Update the eye position to follow a circular path around (0,0,0)
        posX = radius * cos(angle);
        posZ = radius * sin(angle);

        // Update posY based on direction (up or down)
        posY += posYDirection * posYIncrement;

        // Switch direction when reaching the top or bottom of the interval
        if (posYDirection == 1 && posY >= 140) {
            posYDirection = -1; // Switch to moving down
        } else if (posYDirection == -1 && posY <= 100) {
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
    basica();
    return 0;
}
