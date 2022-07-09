#ifndef SPHERE_GLSL
#define SPHERE_GLSL

#include "ray.glsl"

struct Sphere {
    vec3 c;
    float r;
};

bool sphereHit(Sphere self, Ray ray) {
    vec3 oc = ray.o - self.c;
    float a = dot(ray.d, ray.d);
    float b = 2.0 * dot(oc, ray.d);
    float c = dot(oc, oc) - self.r * self.r;
    float discriminant = b * b - 4 * a * c;
    return discriminant > 0;
}

#endif // SPHERE_GLSL