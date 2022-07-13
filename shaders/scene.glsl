#ifndef SCENE_GLSL
#define SCENE_GLSL

#include "sphere.glsl"
#include "aabb.glsl"

#define MAX_OBJECT_NUM 100u

struct Scene {
    uint numObject;
    uint objects[MAX_OBJECT_NUM];

    uint numSphere;
    Sphere spheres[MAX_OBJECT_NUM];
    uint numAabb;
    Aabb aabbs[MAX_OBJECT_NUM];
};

Scene sceneInit() {
    Scene self;
    self.numObject = 0;
    self.numSphere = 0;
    self.numAabb = 0;
    return self;
}

void sceneAddSphere(inout Scene self, Sphere sphere) {
    self.objects[self.numObject] = 0 << 16 | self.numSphere;
    ++self.numObject;
    self.spheres[self.numSphere] = sphere;
    ++self.numSphere;
}

void sceneAddAabb(inout Scene self, Aabb aabb) {
    self.objects[self.numObject] = 1 << 16 | self.numAabb;
    ++self.numObject;
    self.aabbs[self.numAabb] = aabb;
    ++self.numAabb;
}

bool sceneObjectIntersect(Scene self, uint object, Ray ray, out SurfaceInteraction interaction, float tmin, float tmax) {
    switch (object >> 16) {
        case 0x0000:
            return sphereIntersect(self.spheres[object & 0x0000FFFF], ray, interaction, tmin, tmax);
        case 0x0001:
            return aabbIntersect(self.aabbs[object & 0x0000FFFF], ray, interaction, tmin, tmax);
    }
}

bool sceneIntersect(Scene self, Ray ray, out SurfaceInteraction interaction, float tmin, float tmax) {
    bool intersected = false;
    float nearest = tmax;
    SurfaceInteraction temp;
    for (uint i = 0; i < self.numObject; ++i) {
        if (sceneObjectIntersect(self, self.objects[i], ray, temp, tmin, nearest)) {
            intersected = true;
            nearest = temp.t;
            interaction = temp;
        }
    }
    return intersected;
}

#endif // SCENE_GLSL