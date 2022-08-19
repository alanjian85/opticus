#ifndef UTILITY_GLSL
#define UTILITY_GLSL

bool nearZero(vec3 v) {
    const float s = 1e-8;
    return (abs(v.x) < s) && (abs(v.y) < s) && (abs(v.z) < s);
}

#endif // UTILITY_GLSL