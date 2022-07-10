#version 450 core
#extension GL_ARB_shading_language_include : require

#include "/include/ray.glsl"
#include "/include/scene.glsl"
#include "/include/random.glsl"

uniform float aspectRatio;
uniform float fov;
uniform vec3 camPos;
uniform vec3 camRight;
uniform vec3 camUp;
uniform vec3 camFront;

uniform samplerCubeArray skybox;

in vec2 fragTexCoord;

out vec4 outColor;

// the direction of the ray is required to be normalized
// TODO: Support transparent object
vec3 rayColor(Ray ray, Scene scene, int depth) {
    vec3 result = vec3(1.0, 1.0, 1.0);
    while (depth > 0) {
        SurfaceInteraction interaction;
        if (sceneIntersect(scene, ray, interaction, 0.001, FLT_MAX)) {
            if (depth == 1) {
                result = vec3(0.0, 0.0, 0.0);
            } else {
                result *= 0.5;
                ray.o = interaction.p;
                ray.d = normalize(interaction.n + randomInUnitSphere());
            }
        } else {
            result *= texture(skybox, vec4(ray.d, 0.0)).rgb;
            break;
        }
        --depth;
    }
    return result;
}

void main() {
    const float viewportHeight = 2.0;

    const float u = fragTexCoord.x, v = fragTexCoord.y;
    const float focalLength = 0.5 / tan(radians(fov) * 0.5);
    const float viewportWidth = aspectRatio * viewportHeight;

    randomInit();

    Scene scene = sceneInit();
    sceneAddSphere(scene, sphereInit(vec3(0.0, 0.0, -1.0), 0.5));
    sceneAddSphere(scene, sphereInit(vec3(0.0, -100.5, -1.0), 100.0));
    sceneAddAabb(scene, aabbInit(vec3(-1.75, -0.5, -0.5), vec3(-0.75, 0.5, -1.5)));

    Ray ray;
    ray.o = camPos;
    ray.d = normalize(
        (u - 0.5) * camRight * viewportWidth +
        (v - 0.5) * camUp * viewportHeight   +
        camFront * focalLength
    );
    outColor = vec4(rayColor(ray, scene, 50), 1.0);
}