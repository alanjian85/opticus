#ifndef METAL_GLSL
#define METAL_GLSL

struct Metal {
    vec3 albedo;
};

Metal metalInit(vec3 albedo) {
    Metal metal;
    metal.albedo = albedo;
    return metal;
}

bool metalScatter(Metal self, Ray ray, SurfaceInteraction interaction, 
                  out vec3 attenuation, out Ray scattered)
{
    vec3 reflected = reflect(normalize(ray.d), interaction.n);
    scattered = rayInit(interaction.p, reflected);
    attenuation = self.albedo;
    return dot(reflected, interaction.n) > 0.0;
}

#endif // METAL_GLSL