#ifndef DIELECTRIC_GLSL
#define DIELECTRIC_GLSL

#include "/include/core/ray.glsl"

struct Dielectric {
    float refraction;
};

Dielectric dielectricInit(float refraction) {
    Dielectric dielectric;
    dielectric.refraction = refraction;
    return dielectric;
}

bool dielectricScatter(Dielectric self, Ray ray, SurfaceInteraction interaction, 
                       out vec3 attenuation, out Ray scattered)
{
    attenuation = vec3(1.0, 1.0, 1.0);
    float refraction = interaction.front ? (1.0 / self.refraction) : self.refraction;
    vec3 refracted = refract(ray.d, interaction.n, refraction);
    scattered = rayInit(interaction.p, refracted);
    return true;
}

#endif // DIELECTRIC_GLSL