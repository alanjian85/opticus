#ifndef RAY_GLSL
#define RAY_GLSL

struct Ray {
    vec3 o;
    vec3 d;
};

vec3 rayAt(Ray self, float t) {
    return self.o + t * self.d;
}

#endif // RAY_GLSL