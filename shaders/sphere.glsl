#ifndef SPHERE_GLSL
#define SPHERE_GLSL

#include "ray.glsl"

struct Sphere {
    vec3 c;
    float r;
};

float sphereIntersect(Sphere self, Ray ray) {
    vec3 oc = ray.o - self.c;
    float a = dot(ray.d, ray.d);
    float b = dot(oc, ray.d);
    float c = dot(oc, oc) - self.r * self.r;
    float discriminant = b * b - a * c;
    if (discriminant < 0) {
        return -1.0;
    } else {
        return (-b - sqrt(discriminant)) / a;
    }
}

#endif // SPHERE_GLSL