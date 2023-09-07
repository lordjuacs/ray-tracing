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

    pImg = new CImg<BYTE>(w, h, 1, 10);
    CImgDisplay dis_img((*pImg), "Imagen RayTracing en Perspectiva desde una Camara Pinhole");

    /*Esfera esf(vec3(2, 0, 0), 8, vec3(0, 1, 0));
    esf.kd = 0.75;
    esf.ks = 0.8;
    esf.n = 2;
    */
    vector<Objeto *> esferas;
    Objeto *sphere;

    std::random_device rd;
    std::mt19937 gen(rd()); // Mersenne Twister PRNG

    for (int i = 0; i < size; ++i) {
        // Center coordinates
        float centerX = static_cast<float>(std::uniform_real_distribution<double>(-17.0, 17.0)(gen));
        float centerY = static_cast<float>(std::uniform_real_distribution<double>(-17.0, 17.0)(gen));
        float centerZ = static_cast<float>(std::uniform_real_distribution<double>(-17.0, 17.0)(gen));

        // Radius
        float rad = static_cast<float>(std::uniform_real_distribution<double>(1.0, 5)(gen));

        // Color
        int colorR = std::uniform_int_distribution<int>(0, 1)(gen);
        int colorG = std::uniform_int_distribution<int>(0, 1)(gen);
        int colorB = std::uniform_int_distribution<int>(0, 1)(gen);

        //Constants
        float kdt = static_cast<float>(std::uniform_real_distribution<double>(0.0, 1.0)(gen));
        float kst = static_cast<float>(std::uniform_real_distribution<double>(0.0, 1.0)(gen));
        int nt = std::uniform_int_distribution<int>(0, 40)(gen);

        vec3 cen(centerX, centerY, centerZ);
        vec3 col(colorR, colorG, colorB);
        sphere = new Esfera(cen, rad, col);
        sphere->kd = kdt;
        sphere->ks = kst;
        sphere->n = nt;
        esferas.push_back(sphere);
    }

    Luz luz(vec3(30, 30, 30), vec3(1, 1, 1));
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
            //tomar el punto de interseccion mas cercano a la camara
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
                //color = closest->color;
                //ya existe el punto de interseccion pi, el mas cercano a la camara
                vec3 pi = rayo.ori + rayo.dir * t;
                //vector L hacia la luz
                vec3 L = luz.pos - pi;
                L.normalize();

                float ka = 0.3; // constante de reflexion del ambiente
                vec3 l_amb = vec3(1, 1, 1) * ka;

                vec3 l_dif = vec3(0, 0, 0);
                float f_dif = normal.punto(L);//no puede ser negativo ni cero
                if (f_dif > 0)
                    l_dif = luz.color * closest->kd * f_dif;

                vec3 l_spec = vec3(0, 0, 0);
                vec3 R = 2 * (L.punto(normal)) * normal - L;
                R.normalize();
                vec3 V = -rayo.dir; //hacia donde viene el rayo
                V.normalize();

                float f_spec = R.punto(V); // no puede ser negativo ni cero
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
    string nombre_archivo = "imagen" + to_string(size) + "esferas.bmp";
    pImg->save(nombre_archivo.c_str());
    /*while (!dis_img.is_closed()) {
        dis_img.wait();
    }*/

}

