#ifndef RAY_GLSL
#define RAY_GLSL

struct Ray {
    vec3 o;
    vec3 d;
};

Ray rayInit(vec3 o, vec3 d) {
    Ray ray;
    ray.o = o;
    ray.d = d;
    return ray;
}

vec3 rayAt(Ray self, float t) {
    return self.o + t * self.d;
}

#endif // RAY_GLSL