//
// Created by hgallegos on 30/03/2023.
//

#include "Camara.h"
#include "Luz.h"
#include <vector>
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


void Camara::renderizar(vector<Objeto *> &objects, vector<Luz *> &luces, int it) {
    Rayo rayo;
    rayo.ori = eye;
    vec3 dir, color(0, 0, 0);
    pImg = new CImg<BYTE>(w, h, 1, 10);
    CImgDisplay dis_img((*pImg), "Imagen RayTracing en Perspectiva desde una Camara Pinhole");
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            dir = ze * (-f) + ye * a * (y / h - 0.5) + xe * b * (x / w - 0.5);
            dir.normalize();
            rayo.dir = dir;


            color = color_final(rayo, objects, luces, 0); //recursividad color final


            (*pImg)(x, h - 1 - y, 0) = (BYTE) (color.x * 255);
            (*pImg)(x, h - 1 - y, 1) = (BYTE) (color.y * 255);
            (*pImg)(x, h - 1 - y, 2) = (BYTE) (color.z * 255);


        }
    }
    //dis_img.render((*pImg));
    //dis_img.paint();
    string nombre_archivo = "imagen" + to_string(it) + "AVER.bmp";
    pImg->save(nombre_archivo.c_str());
    /*while (!dis_img.is_closed()) {
        dis_img.wait();
    }*/
}

vec3 Camara::color_final(Rayo &rayo, vector<Objeto *> &objects, vector<Luz *> &luces, int depth) {
    vec3 color(0, 0, 0), normal, normal_temp;
    bool exists_obj = false;
    float t = FLT_MAX, t_temp;
    float e = 0.0005;

    Objeto *closest;
    //tomar el punto de interseccion mas cercano a la camara
    //int light_objects = 0;
    for (auto &obj: objects) {
        // if (obj->light)light_objects++;
        if (obj->intersectar(rayo, t_temp, normal_temp)) {
            exists_obj = true;
            if (t_temp < t) {
                t = t_temp;
                normal = normal_temp;
                closest = obj;
            }
        }
    }
    vec3 l_dif = vec3(0, 0, 0);
    vec3 l_spec = vec3(0, 0, 0);

    //si no hay interseccion no alterar el color (negro)
    if (exists_obj && closest->light) {
        color = closest->color;
    } else if (exists_obj) {
        //color = closest->color;
        //ya existe el punto de interseccion pi con un objeto, el mas cercano a la camara
        vec3 pi = rayo.ori + rayo.dir * t;
        vec3 V = -rayo.dir; //hacia donde viene el rayo
        float ka = 0.2; // constante de reflexion del ambiente
        vec3 l_amb = vec3(1, 1, 1) * ka;
        for (auto &luz: luces) {// se va a computar para cada luz que exista
            //vector L hacia la luz
            vec3 L = luz->pos - pi;
            double disL = L.modulo(); //distancia de pi hacia la fuente de luz
            L.normalize();

            //revisar si hay sombra
            bool exists_shadow = false;
            vec3 pis = pi + e * normal; // para que rayo de sombra no se choque con mismo objeto de donde sale
            Rayo rs(pis, L);

            //tomar el punto de interseccion mas cercano a la camara
            for (auto &obj: objects) {
                if (!obj->light && obj->intersectar(rs, t_temp, normal_temp) && !obj->transparency) {// sombra si es transparente??
                    if (t_temp < disL) {
                        exists_shadow = true;//rs choco con un objeto antes de llegar a la luz (foco)
                        break;
                    }
                }
            }

            //para cada fuente de luz, difusa y especular, y hacer promedio de todas
            if (exists_shadow) {
                //color = color + closest->color * l_amb;
                //color = closest->color * prom (lamb1 + lamb2 + lamb3
                //cout << "shadow";
            } else {
                //luz difusa
                float f_dif = normal.punto(L);//no puede ser negativo ni cero
                if (f_dif > 0)//no puede ser negativo ni 0
                    l_dif = l_dif + luz->color * closest->kd * f_dif;
                //luz especular
                vec3 R = 2 * (L.punto(normal)) * normal - L;
                R.normalize();
                //vec3 V = -rayo.dir; //hacia donde viene el rayo
                V.normalize();
                float f_spec = R.punto(V); // no puede ser negativo ni cero
                if (f_spec > 0)
                    l_spec = l_spec + luz->color * closest->ks * pow(f_spec, closest->n);
            }
        }
        //luz total
        color = color + closest->color * (l_amb + l_dif/luces.size() + l_spec/luces.size());
        //color.max_to_one();

        //revisar si objeto es transparente
        if (closest->transparency) {
            if (depth + 1 < max_depth) {
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
                    color_refraction = color_final(refraction_ray, objects, luces, depth + 1);
                }
                vec3 reflection_dir = 2 * (V.punto(normal)) * normal - V;//reflect(dir, normal).normalize();
                reflection_dir.normalize();
                vec3 reflection_ori = outside ? pi + bias : pi - bias;
                Rayo reflection_ray(reflection_ori, reflection_dir);
                vec3 color_reflection = color_final(reflection_ray, objects, luces, depth + 1);

                //mix the two
                color = color + color_reflection * kr + color_refraction * (1 - kr);
            }
        } else {// no es transparente

            //recursividad, espejo de colores hasta la profundidad maxima
            if (closest->ke > 0 && depth + 1 < max_depth) {
                // reflection

                Rayo rr;
                rr.ori = pi + e * normal;
                rr.dir = 2 * (V.punto(normal)) * normal - V;
                rr.dir.normalize();
                vec3 color_reflection = color_final(rr, objects, luces, depth + 1);
                color = color + closest->ke * color_reflection;//casi nunca se refleja el 100%
            }
        }


    }
    color.max_to_one();
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


