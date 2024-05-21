#ifndef SHAPE_GLSL
#define SHAPE_GLSL

#include "/include/core/material.glsl"
#include "/include/core/ray.glsl"

struct SurfaceInteraction {
    float t;
    vec3 p;
    vec3 n;
    bool front;
    uint mat;
};

void interactionSetNormal(inout SurfaceInteraction self, Ray ray, vec3 n) {
    if (dot(n, ray.d) > 0.0) {
        self.n = -n;
        self.front = false;
    } else {
        self.n = n;
        self.front = true;
    }
}

#endif // SHAPE_GLSL