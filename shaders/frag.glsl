#version 450 core
#extension GL_ARB_shading_language_include : require

#include "/include/core/ray.glsl"
#include "/include/core/scene.glsl"
#include "/include/core/random.glsl"

uniform float aspectRatio;
uniform int screenWidth;
uniform int screenHeight;
uniform float fov;
uniform vec3 camPos;
uniform vec3 camRight;
uniform vec3 camUp;
uniform vec3 camFront;

uniform samplerCubeArray skybox;

in vec2 fragTexCoord;

out vec4 outColor;

// TODO: Support transparent objects
vec3 rayColor(Ray ray, Scene scene, int depth) {
    vec3 result = vec3(1.0, 1.0, 1.0);
    while (depth > 0) {
        SurfaceInteraction interaction;
        if (sceneIntersect(scene, ray, interaction, 0.001, FLT_MAX)) {
            if (depth == 1) {
                result = vec3(0.0, 0.0, 0.0);
            } else {
                Ray scattered;
                vec3 attenuation;
                if (sceneMaterialScatter(scene, interaction.mat, ray, interaction, attenuation, scattered)) {
                    result *= attenuation;
                    ray = scattered;
                } else {
                    result = vec3(0.0, 0.0, 0.0);
                    break;
                }
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
    const uint samples = 4;
    const float viewportHeight = 2.0;

    const float u = fragTexCoord.x, v = fragTexCoord.y;
    const float focalLength = 0.5 / tan(radians(fov) * 0.5);
    const float viewportWidth = aspectRatio * viewportHeight;

    randomInit();

    Scene scene = sceneInit();

    Lambertian materialGround = lambertianInit(vec3(0.8, 0.8, 0.0));
    Lambertian materialCenter = lambertianInit(vec3(0.7, 0.3, 0.3));
    Metal materialLeft = metalInit(vec3(0.8, 0.8, 0.8), 0.3);
    Metal materialRight = metalInit(vec3(0.8, 0.6, 0.2), 1.0);

    sceneBindLambertian(scene, sceneAddSphere(scene, sphereInit(vec3(0.0, -100.5, -1.0), 100.0)), materialGround);
    sceneBindLambertian(scene, sceneAddAabb(scene, aabbInit(vec3(-0.5, -0.5, -1.5), vec3( 0.5, 0.5, -0.5))), materialCenter);
    sceneBindMetal(scene, sceneAddSphere(scene, sphereInit(vec3(-1.0, 0.0, -1.0), 0.5)), materialLeft);
    sceneBindMetal(scene, sceneAddSphere(scene, sphereInit(vec3( 1.0, 0.0, -1.0), 0.5)), materialRight);

    vec3 color = vec3(0.0, 0.0, 0.0);
    for (uint i = 0; i < samples; ++i) {
        Ray ray;
        ray.o = camPos;
        ray.d = normalize(
            (u - 0.5 + randomFloat() / float(screenWidth - 1)) * camRight * viewportWidth +
            (v - 0.5 + randomFloat() / float(screenHeight - 1)) * camUp * viewportHeight  +
            camFront * focalLength
        );
        color += rayColor(ray, scene, 50);
    }
    outColor = vec4(color / float(samples), 1.0);

    // https://www.shadertoy.com/view/lsKSWR
    vec2 uv = fragTexCoord;
    uv *= 1.0 - uv.yx;
    float vignette = pow(uv.x * uv.y * 15.0, 0.25);
    outColor.rgb *= vignette;
}