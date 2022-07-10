#ifndef AABB_GLSL
#define AABB_GLSL

#include "limits.glsl"

struct Aabb {
    vec3 pMin;
    vec3 pMax;
};

Aabb aabbInit(vec3 p1, vec3 p2) {
    Aabb self;
    self.pMin = min(p1, p2);
    self.pMax = max(p1, p2);
    return self;
}

bool aabbIntersect(Aabb self, Ray ray, out SurfaceInteraction interaction, float tmin, float tmax) {
    float nearest = tmin, furthest = tmax;
    for (int i = 0; i < 3; ++i) {
        float t0 = min((self.pMin[i] - ray.o[i]) / ray.d[i], (self.pMax[i] - ray.o[i]) / ray.d[i]);
        float t1 = max((self.pMin[i] - ray.o[i]) / ray.d[i], (self.pMax[i] - ray.o[i]) / ray.d[i]);
    
        nearest = max(t0, nearest);
        furthest = min(t1, furthest);
        if (furthest <= nearest)
            return false;
    }
    if (nearest > tmin)
        interaction.t = nearest;
    else
        interaction.t = furthest;
    interaction.p = rayAt(ray, interaction.t);

    vec4 planes[6] = vec4[6](
        vec4( 1.0,  0.0,  0.0, -self.pMax.x),
        vec4(-1.0,  0.0,  0.0,  self.pMin.x),
        vec4( 0.0,  1.0,  0.0, -self.pMax.y),
        vec4( 0.0, -1.0,  0.0,  self.pMin.y),
        vec4( 0.0,  0.0,  1.0, -self.pMax.z),
        vec4( 0.0,  0.0, -1.0,  self.pMin.z)
    );
    nearest = FLT_MAX;
    for (int i = 0; i < 6; ++i) {
        float distance = abs(dot(vec4(interaction.p, 1.0), planes[i]));
        if (distance < nearest) {
            nearest = distance;
            interaction.n = planes[i].xyz;
        }
    }
    return true;
}

#endif // AABB_GLSL