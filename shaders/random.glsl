#ifndef RANDOM_GLSL
#define RANDOM_GLSL

#include "limits.glsl"

#define RAND_MAX UINT_MAX

uint random_uint(inout uint seed) {
    seed = (seed ^ 61) ^ (seed >> 16);
    seed *= 9;
    seed = seed ^ (seed >> 4);
    seed *= 0x27d4eb2d;
    seed = seed ^ (seed >> 15);
    return seed;
}

float random_float(inout uint seed) {
    return float(random_uint(seed)) / float(RAND_MAX);
}

#endif // RANDOM_GLSL