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

float reflectance(float cosine, float refraction) {
    float r0 = (1.0 - refraction) / (1.0 + refraction);
    r0 = r0 * r0;
    return r0 + (1.0 - r0) * pow(1.0 - cosine, 5.0);
}

bool dielectricScatter(Dielectric self, Ray ray, SurfaceInteraction interaction, 
                       out vec3 attenuation, out Ray scattered)
{
    attenuation = vec3(1.0, 1.0, 1.0);
    float refraction = interaction.front ? (1.0 / self.refraction) : self.refraction;
    float cosine = dot(-ray.d, interaction.n);
    float sine = sqrt(1.0 - cosine * cosine);
    vec3 direction;
    if (refraction * sine > 1.0 || reflectance(cosine, refraction) > randomFloat()) {
        direction = reflect(ray.d, interaction.n);
    } else {
        direction = refract(ray.d, interaction.n, refraction);
    }
    scattered = rayInit(interaction.p, direction);
    return true;
}

#endif // DIELECTRIC_GLSL