#ifndef METAL_GLSL
#define METAL_GLSL

struct Metal {
    uint bruh;
};

bool metalScatter(Metal self, Ray ray, SurfaceInteraction interaction, 
                  out vec3 attenuation, out Ray scattered)
{
    return true;
}

#endif // METAL_GLSL