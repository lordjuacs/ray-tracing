#include <iostream>
#include "Camara.h"
#include <cstdlib> // Include this for atoi()

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <size>" << std::endl;
        return 1;
    }

    int size = std::atoi(argv[1]); // Convert the first command-line argument to an integer

    vec3 center = vec3(0,0,0);
    vec3 up = vec3(0,1,0);

    Camara cam;
    cam.configurar(3, 60, 800, 600,
                   vec3(0, 5, 50),
                   center,
                   up);

    cam.renderizar(size);

    return 0;
}
