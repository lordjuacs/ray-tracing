#include <iostream>
#include "Camara.h"

int main() {
    vec3 center = vec3(0,0,0);
    vec3 up = vec3(0,1,0);

    Camara cam;
    cam.configurar(3, 60, 800, 600,
                   vec3(0, 5, 50),
                   center,
                   up);
    cam.renderizar(100);

    return 0;
}
