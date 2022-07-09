#ifndef PRIMITIVE_GLSL
#define PRIMITIVE_GLSL

#include "geometry.glsl"

struct SurfaceInteraction {
    float t;
    vec3 p;
    vec3 n;
};

struct Sphere {
    vec3 c;
    float r;
};

bool sphereIntersect(Sphere self, Ray ray, out SurfaceInteraction interaction, float tmin, float tmax) {
    vec3 oc = ray.o - self.c;
    float a = dot(ray.d, ray.d);
    float b = dot(oc, ray.d);
    float c = dot(oc, oc) - self.r * self.r;
    float discriminant = b * b - a * c;
    if (discriminant < 0) return false;
    float sqrtd = sqrt(discriminant);
    interaction.t =  (-b - sqrtd) / a;
    if (interaction.t < tmin || interaction.t > tmax) {
        interaction.t = (-b + sqrtd) / a;
        if (interaction.t < tmin || interaction.t > tmax) {
            return false;
        }
    }
    interaction.p = rayAt(ray, interaction.t);
    interaction.n = (interaction.p - self.c) / self.r;
    if (dot(interaction.n, ray.d) > 0.0) {
        interaction.n = - interaction.n;
    }
    return true;
}

#endif // PRIMITIVE_GLSL