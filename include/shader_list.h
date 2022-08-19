#ifndef SHADER_LIST_H
#define SHADER_LIST_H

const char* const shaderHeaders[] = {
    "core/ray.glsl",
    "core/limits.glsl",
    "core/random.glsl",
    "core/shape.glsl", 
    "core/material.glsl",
    "core/scene.glsl",
    "shapes/sphere.glsl",
    "shapes/aabb.glsl",
    "materials/lambertian.glsl",
    "materials/metal.glsl"
};

#endif // SHADER_LIST_H