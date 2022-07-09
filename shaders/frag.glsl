#version 450 core
#extension GL_ARB_shading_language_include : require

#include "/include/ray.glsl"
#include "/include/sphere.glsl"

uniform float aspectRatio;
uniform float fov; // radian
uniform vec3 camRight;
uniform vec3 camUp;
uniform vec3 camFront;

in vec2 fragTexCoord;

out vec4 outColor;

// the direction of the ray is required to be normalized
// TODO: Support transparent object
vec3 rayColor(Ray ray) {
    Sphere sphere;
    sphere.c = vec3(0.0, 0.0, -1.0);
    sphere.r = 0.5;
    float t = sphereHit(sphere, ray);
    if (t > 0.0) {
        vec3 normal = normalize(rayAt(ray, t) - sphere.c);
        return 0.5 * vec3(normal.x + 1.0, normal.y + 1.0, normal.z + 1.0);
    }
    t = (ray.d.y + 1.0) * 0.5;
    return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}

void main() {
    // Constants definitions
    const float viewportHeight = 2.0;

    // Main
    const float u = fragTexCoord.x, v = fragTexCoord.y;
    const float focalLength = 0.5 / tan(fov * 0.5);
    const float viewportWidth = aspectRatio * viewportHeight;

    Ray ray;
    ray.o = vec3(0.0, 0.0, 0.0);
    ray.d = normalize(
        (u - 0.5) * camRight * viewportWidth + 
        (v - 0.5) * camUp * viewportHeight - 
        focalLength * camFront - ray.o
    );
    outColor = vec4(rayColor(ray), 1.0);
}