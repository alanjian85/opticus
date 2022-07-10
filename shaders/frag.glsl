#version 450 core
#extension GL_ARB_shading_language_include : require

#include "/include/ray.glsl"
#include "/include/sphere.glsl"
#include "/include/AABB.glsl"
#include "/include/utility.glsl"

uniform float aspectRatio;
uniform float fov; // radian
uniform vec3 camRight;
uniform vec3 camUp;
uniform vec3 camFront;
uniform vec3 camPos;
uniform samplerCubeArray skybox;

in vec2 fragTexCoord;

out vec4 outColor;

// the direction of the ray is required to be normalized
// TODO: Support transparent object
vec3 rayColor(Ray ray) {
    SurfaceInteraction interaction;
    if (aabbIntersect(aabbInit(vec3(-0.5, -0.5, -0.5), vec3(0.5, 0.5, -1.5)), ray, interaction, 0.0, FLT_MAX)) {
        return 0.5 * vec3(interaction.n.x + 1.0, interaction.n.y + 1.0, interaction.n.z + 1.0);
    }
    return texture(skybox, vec4(ray.d, 0.0)).rgb;
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