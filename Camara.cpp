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
    vector<Objeto *> objects;
    Objeto *sphere;

    std::random_device rd;
    std::mt19937 gen(rd()); // Mersenne Twister PRNG
    int si_son_t = 0;
    for (int i = 0; i < size; ++i) {
        // Center coordinates
        float centerX = static_cast<float>(std::uniform_real_distribution<double>(-17.0, 17.0)(gen));
        float centerY = static_cast<float>(std::uniform_real_distribution<double>(-17.0, 17.0)(gen));
        float centerZ = static_cast<float>(std::uniform_real_distribution<double>(-17.0, 17.0)(gen));

        // Radius
        float rad = static_cast<float>(std::uniform_real_distribution<double>(1.0, 5)(gen));

        // Color
        float colorR = static_cast<float>(std::uniform_real_distribution<double>(0.1, 1.0)(gen));
        float colorG = static_cast<float>(std::uniform_real_distribution<double>(0.1, 1.0)(gen));
        float colorB = static_cast<float>(std::uniform_real_distribution<double>(0.1, 1.0)(gen));

        //Constants
        float kdt = static_cast<float>(std::uniform_real_distribution<double>(0.0, 1.0)(gen));
        float kst = static_cast<float>(std::uniform_real_distribution<double>(0.0, 1.0)(gen));
        int nt = std::uniform_int_distribution<int>(0, 40)(gen);
        float ket = static_cast<float>(std::uniform_real_distribution<double>(0.0, 1.0)(gen));
        bool transpt = std::uniform_int_distribution<int>(0, 1)(gen);
        float iort = static_cast<float>(std::uniform_real_distribution<double>(0.0, 1.0)(gen));

        vec3 cen(centerX, centerY, centerZ);
        vec3 col(colorR, colorG, colorB);
        sphere = new Esfera(cen, rad, col);
        sphere->init_constants(kdt, kst, nt, ket, transpt, iort);

        objects.push_back(sphere);
        if(transpt) si_son_t++;
    }
    //std::cout << "Transparentes: " << si_son_t;
    Objeto *p1 = new Plano(vec3(0, 1, 0), 1, vec3(0.123, 0.456, 0.789));
    p1->init_constants(0.9, 0.1);
    p1->ke = 0.1;
    objects.emplace_back(p1);
    /*float colorR = static_cast<float>(std::uniform_real_distribution<double>(0.1, 1.0)(gen));
    float colorG = static_cast<float>(std::uniform_real_distribution<double>(0.1, 1.0)(gen));
    float colorB = static_cast<float>(std::uniform_real_distribution<double>(0.1, 1.0)(gen));

    auto p1 = new Plano(vec3(0, 1, 0), 1, vec3(colorR, colorG, colorB));
    p1->setConstantes(0.9, 0.1);
    objects.emplace_back(p1);*/
    /*Objeto *p1;
    p1 = new Esfera(vec3(10, 0, 0), 8, vec3(0, 0, 1));
    p1->init_constants(1, 0);
    p1->ke = 1;
    objects.emplace_back(p1);
    p1 = new Esfera(vec3(10, 0, 20), 8, vec3(0.1, 0.1, 0.1));
    p1->init_constants(0.8, 0.2);
    p1->ke = 0.9;
    p1->transparency = true;
    p1->ior = 1.5;
    objects.emplace_back(p1);
    p1 = new Esfera(vec3(0, 10, 0), 8, vec3(0, 0, 0));
    p1->init_constants(0.6, 0.4, 32);
    p1->ke = 0.8;
    p1->transparency = false;
    p1->ior = 1.2;
    objects.emplace_back(p1);

    p1 = new Plano(vec3(0, 1, 0), 1, vec3(0.123, 0.456, 0.789));
    p1->init_constants(0.9, 0.1);
    p1->ke = 0.1;
    objects.emplace_back(p1);

    p1 = new Cilindro(vec3(-20, 0, 0), vec3(-10, 10, 0), 5, vec3(0, 1, 1));
    p1->init_constants(0.7, 0.3, 8);
    objects.emplace_back(p1);
    vector<Luz *> luces;
    //Luz luz(vec3(30,30,30), vec3(1,1,1));
    //luces.emplace_back(&luz);
    */
    // fuente de luz (foco)
    Luz *luz = new Luz(vec3(30, 30, 30), vec3(1, 1, 1));
    vec3 color;

    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            dir = ze * (-f) + ye * a * (y / h - 0.5) + xe * b * (x / w - 0.5);
            dir.normalize();
            rayo.dir = dir;
            color = color_final(rayo, objects, luz, 0);//recursividad color final
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

vec3 Camara::color_final(Rayo rayo, vector<Objeto *> objects, Luz *luz, int depth) {
    vec3 color(0, 0, 0), normal, normal_temp;
    bool exists_obj = false;
    float t = FLT_MAX, t_temp;
    Objeto *closest;
    //tomar el punto de interseccion mas cercano a la camara
    for (auto obj: objects) {
        if (obj->intersectar(rayo, t_temp, normal_temp)) {
            exists_obj = true;
            if (t_temp < t) {
                t = t_temp;
                normal = normal_temp;
                closest = obj;
            }
        }
    }
    //si no hay interseccion no alterar el color (negro)
    if (exists_obj) {
        //color = closest->color;
        //ya existe el punto de interseccion pi con un objeto, el mas cercano a la camara
        vec3 pi = rayo.ori + rayo.dir * t;
        //vector L hacia la luz
        vec3 L = luz->pos - pi;
        double disL = L.modulo(); //distancia de pi hacia la fuente de luz
        L.normalize();

        //revisar si hay sombra
        bool exists_shadow = false;
        float e = 0.0005;
        vec3 pis = pi + e * normal; // para que rayo de sombra no se choque con mismo objeto de donde sale
        Rayo rs(pis, L);

        //tomar el punto de interseccion mas cercano a la camara
        for (auto obj: objects) {
            if (obj->intersectar(rs, t_temp, normal_temp)) {
                if (t_temp <= disL) {
                    exists_shadow = true;//rs choco con un objeto antes de llegar a la luz (foco)
                    break;
                }
            }
        }

        float ka = 0.2; // constante de reflexion del ambiente
        vec3 l_amb = vec3(1, 1, 1) * ka;

        if (exists_shadow)
            color = closest->color * l_amb;
        else {
            //luz difusa
            vec3 l_dif = vec3(0, 0, 0);
            float f_dif = normal.punto(L);//no puede ser negativo ni cero
            if (f_dif > 0)//no puede ser negativo ni 0
                l_dif = luz->color * closest->kd * f_dif;
            //luz especular
            vec3 l_spec = vec3(0, 0, 0);
            vec3 R = 2 * (L.punto(normal)) * normal - L;
            R.normalize();
            vec3 V = -rayo.dir; //hacia donde viene el rayo
            V.normalize();
            float f_spec = R.punto(V); // no puede ser negativo ni cero
            if (f_spec > 0)
                l_spec = luz->color * closest->ks * pow(f_spec, closest->n);
            //luz total
            color = closest->color * (l_amb + l_dif + l_spec);
            //revisar si objeto es transparente
            if (closest->transparency) {
                vec3 color_refraction(0, 0, 0);
                float kr;
                fresnel(rayo.dir, normal, closest->ior, kr);
                bool outside = rayo.dir.punto(normal) < 0;
                vec3 bias = e * normal;
                // compute refraction if it is not a case of total internal reflection
                if (kr < 1) {
                    vec3 refraction_dir = refract(rayo.dir, normal, closest->ior);
                    refraction_dir.normalize();
                    vec3 refraction_ori = outside ? pi - bias : pi + bias;
                    Rayo refraction_ray(refraction_ori, refraction_dir);
                    color_refraction = color_final(refraction_ray, objects, luz, depth + 1);
                }
                vec3 reflection_dir = 2 * (V.punto(normal)) * normal - V;//reflect(dir, normal).normalize();
                reflection_dir.normalize();
                vec3 reflection_ori = outside ? pi + bias : pi - bias;
                Rayo reflection_ray(reflection_ori, reflection_dir);
                vec3 color_reflection = color_final(reflection_ray, objects, luz, depth + 1);

                //mix the two
                color = color + color_reflection * kr + color_refraction * (1 - kr);
            } else {// no es transparente
                //recursividad, espejo de colores hasta la profundidad maxima
                if (closest->ke > 0 && depth + 1 < max_depth) {
                    // reflection
                    Rayo rr;
                    rr.ori = pi + e * normal;
                    rr.dir = 2 * (V.punto(normal)) * normal - V;
                    rr.dir.normalize();
                    vec3 color_reflection = color_final(rr, objects, luz, depth + 1);
                    color = color + closest->ke * color_reflection;//casi nunca se refleja el 100%
                }
            }
            color.max_to_one();
        }
    }
    return color;
}

vec3 Camara::refract(vec3 &I, vec3 &N, float &ior) {
    float cosi = clamp(-1, 1, I.punto(N));
    float etai = 1, etat = ior;
    vec3 n = N;
    if (cosi < 0) { cosi = -cosi; }
    else {
        std::swap(etai, etat);
        n = -N;
    }
    float eta = etai / etat;
    float k = 1 - eta * eta * (1 - cosi * cosi);
    return k < 0 ? vec3(0, 0, 0) : eta * I + (eta * cosi - sqrtf(k)) * n;
}

void Camara::fresnel(vec3 &I, vec3 &N, float &ior, float &kr) {
    float cosi = clamp(-1, 1, I.punto(N));
    float etai = 1, etat = ior;
    if (cosi > 0) { std::swap(etai, etat); }
    // Compute sini using Snell's law
    float sint = etai / etat * sqrtf(std::max(0.f, 1 - cosi * cosi));
    // Total internal reflection
    if (sint >= 1) {
        kr = 1;
    } else {
        float cost = sqrtf(std::max(0.f, 1 - sint * sint));
        cosi = fabsf(cosi);
        float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
        float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
        kr = (Rs * Rs + Rp * Rp) / 2;
    }
    // As a consequence of the conservation of energy, the transmittance is given by:
    // kt = 1 - kr;
}