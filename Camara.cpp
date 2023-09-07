//
// Created by hgallegos on 30/03/2023.
//

#include "Camara.h"
#include "Luz.h"
#include <vector>
#include <random>

using namespace std;

void Camara::configurar(float _near, float fov, int ancho, int alto,
                        vec3 pos_eye, vec3 center, vec3 up) {
    f = _near;
    w = ancho;
    h = alto;
    a = 2 * f * tan(fov * M_PI / 360);
    b = w / h * a;
    eye = pos_eye;
    ze = eye - center;
    ze.normalize();
    xe = up.cruz(ze);
    xe.normalize();
    ye = ze.cruz(xe);
}

void Camara::renderizar(int size) {
    Rayo rayo;
    rayo.ori = eye;
    vec3 dir;

    pImg = new CImg<BYTE>(w, h, 1, 3);
    CImgDisplay dis_img((*pImg), "Imagen RayTracing en Perspectiva desde una Camara Pinhole");

    /*Esfera esf(vec3(2, 0, 0), 8, vec3(0, 1, 0));
    esf.kd = 0.75;
    esf.ks = 0.8;
    esf.n = 2;
    */
    float n = 3; //shininess exponent

    std::random_device rd;
    std::mt19937 gen(rd()); // Mersenne Twister PRNG
    std::uniform_real_distribution<float> floatDist(0.0f, 1.0f);
    std::uniform_int_distribution<int> coordDist(-50, 50);
    std::uniform_int_distribution<int> radiusDist(1, 10);
    std::uniform_int_distribution<int> colorDist(0, 255);
    float xr, yr, zr, kdr, ksr;
    int red, green, blue, radius;
    vector<Objeto *> esferas;
    Objeto *sphere = nullptr;

    for (int i = 0; i < size; i++) {
        xr = static_cast<float>(coordDist(gen));
        yr = static_cast<float>(coordDist(gen));
        zr = static_cast<float>(coordDist(gen));
        radius = radiusDist(gen);
        red = colorDist(gen);
        green = colorDist(gen);
        blue = colorDist(gen);
        kdr = floatDist(gen);
        ksr = floatDist(gen);
        sphere = new Esfera(vec3(xr, yr, zr), radius, vec3(red, green, blue));
        sphere->kd = kdr;
        sphere->ks = ksr;
        sphere->n = n;
        esferas.push_back(sphere);
    }


    Luz luz(vec3(10, 10, 10), vec3(1, 1, 1));
    vec3 color, normal, normal_temp;
    bool exists_obj;
    float t, t_temp;
    Objeto *closest = nullptr;

    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            dir = ze * (-f) + ye * a * (y / h - 0.5) + xe * b * (x / w - 0.5);
            dir.normalize();
            rayo.dir = dir;
            color.set(0, 0, 0);
            exists_obj = false;
            t = FLT_MAX;
            for (auto esf: esferas) {
                if (esf->intersectar(rayo, t_temp, normal_temp)) {
                    exists_obj = true;
                    if (t_temp < t) {
                        t = t_temp;
                        normal = normal_temp;
                        closest = esf;
                    }
                }
            }
            if (exists_obj) {
                //color = esf.color;
                //ya existe el punto de interseccion pi, el mas cercano a la camara
                vec3 pi = rayo.ori + t * rayo.dir;
                //vector L hacia la luz
                vec3 L = luz.pos - pi;
                L.normalize();

                float ka = 0.45; // constante de reflexion del ambiente
                vec3 l_amb = vec3(1, 1, 1) * ka;

                vec3 l_dif = vec3(0, 0, 0);
                float f_dif = normal.punto(L);//no puede ser negativo o cero
                if (f_dif > 0)
                    l_dif = luz.color * closest->kd * f_dif;

                vec3 R = 2 * (L.punto(normal)) * normal - L;
                R.normalize();
                vec3 V = -rayo.dir; //hacia donde viene el rayo
                V.normalize();

                vec3 l_spec = vec3(0, 0, 0);
                float f_spec = R.punto(V); // no puede ser negativo
                if (f_spec > 0)
                    l_spec = luz.color * closest->ks * pow(f_spec, closest->n);
                color = closest->color * (l_amb + l_dif + l_spec);
                color.max_to_one();

            }
            (*pImg)(x, h - 1 - y, 0) = (BYTE) (color.x * 255);
            (*pImg)(x, h - 1 - y, 1) = (BYTE) (color.y * 255);
            (*pImg)(x, h - 1 - y, 2) = (BYTE) (color.z * 255);
        }
    }
    dis_img.render((*pImg));
    dis_img.paint();
    string nombre_archivo = "imagen" + to_string(1) + ".bmp";
    pImg->save(nombre_archivo.c_str());
    while (!dis_img.is_closed()) {
        dis_img.wait();
    }

}

