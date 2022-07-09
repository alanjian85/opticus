#version 450 core
#extension GL_ARB_shading_language_include : require

#include "/include/geometry.glsl"
#include "/include/primitives.glsl"
#include "/include/utility.glsl"

uniform float aspectRatio;
uniform float fov; // radian
uniform vec3 camRight;
uniform vec3 camUp;
uniform vec3 camFront;
uniform vec3 camPos;

in vec2 fragTexCoord;

out vec4 outColor;

// the direction of the ray is required to be normalized
// TODO: Support transparent object
vec3 rayColor(Ray ray) {
    Sphere sphere;
    sphere.c = vec3(0.0, 0.0, -1.0);
    sphere.r = 0.5;
    SurfaceInteraction interaction;
    if (sphereIntersect(sphere, ray, interaction, 0.0, FLT_MAX)) {
        return 0.5 * vec3(interaction.n.x + 1.0, interaction.n.y + 1.0, interaction.n.z + 1.0);
    }
    float t = (ray.d.y + 1.0) * 0.5;
    return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}

void main() {
    const float viewportHeight = 2.0;

    const float u = fragTexCoord.x, v = fragTexCoord.y;
    const float focalLength = 0.5 / tan(fov * 0.5);
    const float viewportWidth = aspectRatio * viewportHeight;

    Ray ray;
    ray.o = camPos;
    ray.d = normalize(
        (u - 0.5) * camRight * viewportWidth +
        (v - 0.5) * camUp * viewportHeight   +
        focalLength * camFront
    );
    outColor = vec4(rayColor(ray), 1.0);
}