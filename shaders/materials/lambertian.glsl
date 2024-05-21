#ifndef LAMBERTIAN_GLSL
#define LAMBERTIAN_GLSL

#include "/include/core/random.glsl"
#include "/include/core/ray.glsl"
#include "/include/core/shape.glsl"
#include "/include/core/utility.glsl"

struct Lambertian {
    vec3 albedo;
};

Lambertian lambertianInit(vec3 albedo) {
    Lambertian lambertian;
    lambertian.albedo = albedo;
    return lambertian;
}

bool lambertianScatter(Lambertian self, Ray ray, SurfaceInteraction interaction,
                       out vec3 attenuation, out Ray scattered) {
    vec3 scattered_dir = interaction.n + randomUnitVector();
    if (nearZero(scattered_dir)) {
        scattered_dir = interaction.n;
    }
    scattered = rayInit(interaction.p, normalize(scattered_dir));
    attenuation = self.albedo;
    return true;
}

#endif // LAMBERTIAN_GLSL
