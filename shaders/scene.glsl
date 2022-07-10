#ifndef SCENE_GLSL
#define SCENE_GLSL

#include "sphere.glsl"
#include "aabb.glsl"

#define MAX_OBJECT_NUM 100

struct Scene {
    uint numSphere;
    Sphere spheres[MAX_OBJECT_NUM];
    uint numAabb;
    Aabb aabbs[MAX_OBJECT_NUM];
};

Scene sceneInit() {
    Scene self;
    self.numSphere = 0;
    self.numAabb = 0;
    return self;
}

void sceneAddSphere(inout Scene self, Sphere sphere) {
    self.spheres[self.numSphere] = sphere;
    ++self.numSphere;
}

void sceneAddAabb(inout Scene self, Aabb aabb) {
    self.aabbs[self.numAabb] = aabb;
    ++self.numAabb;
}

bool sceneIntersect(Scene self, Ray ray, out SurfaceInteraction interaction, float tmin, float tmax) {
    bool intersected = false;
    float nearest = tmax;
    SurfaceInteraction temp;
    for (uint i = 0; i < self.numSphere; ++i) {
        if (sphereIntersect(self.spheres[i], ray, temp, tmin, nearest)) {
            intersected = true;
            nearest = temp.t;
            interaction = temp;
        }
    }
    for (uint i = 0; i < self.numAabb; ++i) {
        if (aabbIntersect(self.aabbs[i], ray, temp, tmin, nearest)) {
            intersected = true;
            nearest = temp.t;
            interaction = temp;
        }
    }
    return intersected;
}

#endif // SCENE_GLSL