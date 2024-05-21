#ifndef SPHERE_GLSL
#define SPHERE_GLSL

#include "/include/core/ray.glsl"
#include "/include/core/shape.glsl"

struct Sphere {
    vec3 c;
    float r;
    bool reverse;
};

Sphere sphereInit(vec3 c, float r, bool reverse) {
    Sphere self;
    self.c = c;
    self.r = r;
    self.reverse = reverse;
    return self;
}

bool sphereIntersect(Sphere self, Ray ray, out SurfaceInteraction interaction,
                     float tmin, float tmax) {
    vec3 oc = ray.o - self.c;
    float a = dot(ray.d, ray.d);
    float b = dot(oc, ray.d);
    float c = dot(oc, oc) - self.r * self.r;
    float discriminant = b * b - a * c;
    if (discriminant < 0)
        return false;
    float sqrtd = sqrt(discriminant);
    interaction.t = (-b - sqrtd) / a;
    if (interaction.t < tmin || interaction.t > tmax) {
        interaction.t = (-b + sqrtd) / a;
        if (interaction.t < tmin || interaction.t > tmax) {
            return false;
        }
    }
    interaction.p = rayAt(ray, interaction.t);
    vec3 normal = (interaction.p - self.c) / self.r;
    interactionSetNormal(interaction, ray, self.reverse ? -normal : normal);
    return true;
}

#endif // SPHERE_GLSL
