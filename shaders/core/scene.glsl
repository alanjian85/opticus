#ifndef SCENE_GLSL
#define SCENE_GLSL

#include "/include/shapes/sphere.glsl"
#include "/include/shapes/aabb.glsl"
#include "/include/materials/lambertian.glsl"
#include "/include/materials/metal.glsl"
#include "/include/materials/dielectric.glsl"

#define MAX_OBJECT_NUM 100u

struct Scene {
    uint numObject;
    uint objects[MAX_OBJECT_NUM];
    uint materials[MAX_OBJECT_NUM];

    uint numSphere;
    Sphere spheres[MAX_OBJECT_NUM];
    uint numAabb;
    Aabb aabbs[MAX_OBJECT_NUM];

    uint numLambertian;
    Lambertian lambertians[MAX_OBJECT_NUM];
    uint numMetal;
    Metal metals[MAX_OBJECT_NUM];
    uint numDielectric;
    Dielectric dielectrics[MAX_OBJECT_NUM];
};

Scene sceneInit() {
    Scene self;
    self.numObject = 0;

    self.numSphere = 0;
    self.numAabb = 0;
    
    self.numLambertian = 0;
    self.numMetal = 0;

    return self;
}

uint sceneAddSphere(inout Scene self, Sphere sphere) {
    self.objects[self.numObject] = 0 << 16 | self.numSphere;
    self.spheres[self.numSphere] = sphere;
    self.numSphere++;
    return self.numObject++;
}

uint sceneAddAabb(inout Scene self, Aabb aabb) {
    self.objects[self.numObject] = 1 << 16 | self.numAabb;
    self.aabbs[self.numAabb] = aabb;
    ++self.numAabb;
    return self.numObject++;
}

void sceneBindLambertian(inout Scene self, uint index, Lambertian lambertian) {
    self.materials[index] = 0 << 16 | self.numLambertian;
    self.lambertians[self.numLambertian] = lambertian;
    self.numLambertian++;
}

void sceneBindMetal(inout Scene self, uint index, Metal metal) {
    self.materials[index] = 1 << 16 | self.numMetal;
    self.metals[self.numMetal] = metal;
    self.numMetal++;
}

void sceneBindDielectric(inout Scene self, uint index, Dielectric dielectric) {
    self.materials[index] = 2 << 16 | self.numDielectric;
    self.dielectrics[self.numDielectric] = dielectric;
    self.numDielectric++;
}

bool sceneObjectIntersect(Scene self, uint object, Ray ray, out SurfaceInteraction interaction, float tmin, float tmax) {
    switch (object >> 16) {
        case 0x0000:
            return sphereIntersect(self.spheres[object & 0xFFFF], ray, interaction, tmin, tmax);
        case 0x0001:
            return aabbIntersect(self.aabbs[object & 0xFFFF], ray, interaction, tmin, tmax);
    }
}

bool sceneMaterialScatter(Scene self, uint material, Ray ray, SurfaceInteraction interaction, out vec3 attenuation, out Ray scattered) {
    switch (material >> 16) {
        case 0x0000:
            return lambertianScatter(self.lambertians[material & 0xFFFF], ray, interaction, attenuation, scattered);
        case 0x0001:
            return metalScatter(self.metals[material & 0xFFFF], ray, interaction, attenuation, scattered);
        case 0x0002:
            return dielectricScatter(self.dielectrics[material & 0xFFFF], ray, interaction, attenuation, scattered);
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
            interaction.mat = self.materials[i];
        }
    }
    return intersected;
}

#endif // SCENE_GLSL