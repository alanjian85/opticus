#ifndef METAL_GLSL
#define METAL_GLSL

struct Metal {
    vec3 albedo;
    float fuzz;
};

Metal metalInit(vec3 albedo, float fuzz) {
    Metal metal;
    metal.albedo = albedo;
    metal.fuzz = fuzz;
    return metal;
}

bool metalScatter(Metal self, Ray ray, SurfaceInteraction interaction, 
                  out vec3 attenuation, out Ray scattered)
{
    vec3 reflected = reflect(ray.d, interaction.n);
    scattered = rayInit(interaction.p, normalize(reflected + self.fuzz * randomUnitSphere()));
    attenuation = self.albedo;
    return dot(reflected, interaction.n) > 0.0;
}

#endif // METAL_GLSL