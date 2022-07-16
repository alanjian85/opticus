#ifndef RANDOM_GLSL
#define RANDOM_GLSL

#include "/include/limits.glsl"

#define RAND_MAX UINT_MAX

uint hash(uint seed) {
    seed = (seed ^ 61u) ^ (seed >> 16u);
    seed *= 9u;
    seed = seed ^ (seed >> 4u);
    seed *= 0x27D4EB2Du;
    seed = seed ^ (seed >> 15u);
    return seed;
}

uniform uint frame;

uint seed;

void randomInit() {
    seed = hash(hash(uint(gl_FragCoord.x) + hash(uint(gl_FragCoord.y))) + frame);
}

uint randomUint() {
    seed = hash(seed);
    return seed;
}

float randomFloat() {
    return float(randomUint()) / float(RAND_MAX);
}

float randomFloat(float min, float max) {
    return min + (max - min) * randomFloat();
}

vec3 randomUnitSphere() {
    while (true) {
        vec3 p = vec3(randomFloat(-1.0, 1.0), randomFloat(-1.0, 1.0), randomFloat(-1.0, 1.0));
        if (dot(p, p) < 1.0)
            return p;
    }
}

vec3 randomHemiSphere(vec3 normal) {
    vec3 p = randomUnitSphere();
    if (dot(p, normal) > 0.0) {
        return p;
    } else {
        return -p;
    }
}

#endif // RANDOM_GLSL