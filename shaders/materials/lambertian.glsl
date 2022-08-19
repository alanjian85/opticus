#ifndef LAMBERTIAN_GLSL
#define LAMBERTIAN_GLSL

struct Lambertian {
    uint bruh;
};

bool lambertianScatter(Lambertian self, Ray ray, SurfaceInteraction interaction, 
                       out vec3 attenuation, out Ray scattered)
{
    return true;
}

#endif // LAMBERTIAN_GLSL