#version 450 core
#extension GL_ARB_shading_language_include : require

#include "/include/ray.glsl"

uniform float aspectRatio;

in vec2 fragTexCoord;

out vec4 outColor;

// the direction of the ray is required to be normalized
// TODO: Support transparent object
vec3 rayColor(Ray ray) {
    float t = (ray.d.y + 1.0) * 0.5;
    return t * vec3(1.0, 1.0, 1.0) + (1.0 - t) * vec3(0.5, 0.7, 1.0);
}

void main() {
    // Constants definitions
    const float viewportHeight = 2.0;
    const float focalLength = 1.0;

    // Main
    const float u = fragTexCoord.x, v = fragTexCoord.y;
    const float viewportWidth = aspectRatio * viewportHeight;

    Ray ray;
    ray.o = vec3(0.0, 0.0, 0.0);
    ray.d = normalize(
        (u - 0.5) * vec3(viewportWidth, 0.0, 0.0) + 
        (v - 0.5) * vec3(0.0, viewportHeight, 0.0) - 
        vec3(0.0, 0.0, focalLength) - ray.o
    );
    outColor = vec4(rayColor(ray), 1.0);
}